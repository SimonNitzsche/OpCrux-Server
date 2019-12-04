#define HOST_ENDIAN_IS_BIG
#define BIG_ENDIAN

#include "Common/HardConfig.hpp"

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Common/CrossPlatform.hpp"

#include <RakNet/RakSleep.h>

#include "Server/MasterServer.hpp"
#include "Server/AuthServer.hpp"
#include "Server/WorldServer.hpp"
#include "Server/Bridges/BridgeMasterServer.hpp"
#include "WebInterface/WebInterface.hpp"
#include "GameCache/Interface/FastDatabase.hpp"
#include "DataTypes/AMF3.hpp"

#include "Utils/FileUtils.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"
#include <chrono>

#include "Database/Database.hpp"

using namespace std::chrono;

std::vector<ILUServer *> virtualServerInstances;

enum class SERVERMODE : uint8_t { STANDALONE, MASTER, WORLD, AUTH } MODE_SERVER;

// Following Includes are for testing
#include "FileTypes/LUZFile/LUZone.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/WorldConfig.hpp"

#include "DataTypes/LDF.hpp"

GameCache::Interface::FDB::Connection Cache;
BridgeMasterServer* masterServerBridge;

#include "Entity/Components/StatsComponent.hpp"

int givenWorldID = 1100;


class MissionRequirementParser {
private:
	std::uint32_t inputIndex = 0;
	std::uint32_t openBrackets = 0;
	static const std::uint32_t openBracketsLimit = 12;
	bool isOperatorOR = false; // 0 = AND, 1 = OR
public:
	bool result = false;
	bool parseSucceeded = false;
private:

	void err(const char* msg) {
		parseSucceeded = false;
		std::cout << msg << std::endl;
	}

	inline bool hasMissionFromBuffer(std::string_view missionAndOrTask) {

		std::cout << "HasMisFromBuf: \"" << missionAndOrTask << "\"" << std::endl;

		return true;
	}

	inline bool doBitOperation(bool leftHand, bool rightHand) {
		return isOperatorOR ? (leftHand | rightHand) : (leftHand & rightHand);
	}

	bool parse(std::string_view statement) {
		// In case we ran into an error before
		if (!parseSucceeded) return false;

		bool bufferResult = true;
		std::uint32_t bufferBegin = inputIndex;
		std::uint32_t bufferLength = 0;

		for (; inputIndex < statement.length(); ++inputIndex) {
			switch (statement.at(inputIndex)) {
			case ' ': {
				// WHITE SPACE
				++bufferLength; // We will deal with it later
				break;
			}
			case ',':
			case '&':
			case '|': {

				if (bufferLength != 0) {
					// set operator
					isOperatorOR = (statement.at(inputIndex) == '|');
					// perform operation
					bufferResult = doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
					// reset buffer
					bufferBegin = inputIndex + 1;
					bufferLength = 0;
				}

				break;
			}
			case '(': {
				// Bracket open
				
				if (openBrackets < openBracketsLimit) {
					++openBrackets;

					// move forward
					++inputIndex;

					// perform operation
					bufferResult = doBitOperation(bufferResult, parse(statement));

					// Check if recursion caused error
					if (!parseSucceeded) return false;

					// reset buffer
					bufferBegin = inputIndex + 1;
					bufferLength = 0;
				}
				else {
					err("Too many brackets opened.");
					return false;
				}
				
				break;
			}
			case ')': {
				// Bracket close

				if (openBrackets != 0) {
					--openBrackets;

					// perform operation
					return doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
				}
				else {
					err("Tried to close bracket without an open bracket");
					return false;
				}

				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ':': {
				// LITERAL
				++bufferLength;
				break;
			}
			default: {
				err("Unknown operator: " + statement.at(inputIndex));
				return false;
			}
			}
		}
		
		// End of statement
		if (parseSucceeded) {
			if (bufferLength != 0) {
				bufferResult = doBitOperation(bufferResult, hasMissionFromBuffer(statement.substr(bufferBegin, bufferLength)));
			}
			return bufferResult;
		}

		// Something went wrong
		return false;
	}

public:

	static std::list<std::string_view> sweepMissionList(std::string_view statement) {
		std::list<std::string_view> result = {};
		std::uint32_t buffer_begin = 0;
		std::uint32_t buffer_end = 0;
		for (int i = 0; i < statement.size(); ++i) {
			switch (statement.at(i)) {

				// Accept [0-9,:]
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case ':': {
				++buffer_end;
				break;
			}

					  // Ignore everything else
			default: {
				if (buffer_end != 0) {
					std::string_view newEntry = statement.substr(buffer_begin, buffer_end);

					// Check for duplicates
					bool isDuplicate = false;
					if (result.size() != 0) {
						auto resultEnd = result.end();
						auto newEntryBegin = newEntry.begin();
						for (auto it = result.begin(); it != resultEnd; ++it) {
							if (*it == newEntry) {
								isDuplicate = true;
								break;
							}
						}
					}
					if (!isDuplicate) {
						result.push_back(newEntry);
					}
					buffer_end = 0;
				}
				buffer_begin = i;
				++buffer_begin;
				break;
			}
			}
		}
		return result;
	}


	MissionRequirementParser(std::string_view statement) {
		parseSucceeded = true;

		result = parse(statement);
	}
};

int main(int argc, char* argv[]) {
	FileUtils::ChangeDirectory();
	std::string ipMaster = "127.0.0.1";
	//std::string ipMaster = "foxsog.com";

	Database::Connect();
	//Database::DoATestQuery();

	auto mrp_test = MissionRequirementParser("44|33&22,99,(11|55)");

	Cache.Connect("./res/cdclient.fdb");

	using namespace Entity;

	MODE_SERVER = SERVERMODE::STANDALONE;

#ifdef OPCRUX_PLATFORM_WIN32
	std::system("title OpCrux Server (Standalone)");
#endif
	for (std::ptrdiff_t i = 0; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (arg == "--master") {
			MODE_SERVER = SERVERMODE::MASTER;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (Master only)");
#endif
		}
		else if (arg == "--world") {
			MODE_SERVER = SERVERMODE::WORLD;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (World only)");
#endif
		}
		else if (arg == "--auth") {
			MODE_SERVER = SERVERMODE::AUTH;
#ifdef OPCRUX_PLATFORM_WIN32
			std::system("title OpCrux Server (Auth only)");
#endif
		}
		else if (arg == "--worldID" && i < argc) {
			givenWorldID = std::stoi(argv[i + 1]);
		}

		if (argc >= 4) {
			ipMaster = argv[i + 1];
		}
	}

	ServerInfo::init();

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		std::thread mT([](MasterServer * ms) { ms = new MasterServer(); }, ServerInfo::masterServer);
		mT.detach();

		std::thread wiT([]() { WebInterface::WebInterfaceLoop(); });
		wiT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		masterServerBridge = new BridgeMasterServer(ipMaster);
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		AuthServer * aS;
		std::thread aT([](AuthServer * as) { as = new AuthServer(); }, aS);
		aT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {
		WorldServer * charSelectWs;
		std::thread wT([](WorldServer * ws) { ws = new WorldServer(givenWorldID, 0,2001); }, charSelectWs);
		wT.detach();
	}

	while (ServerInfo::bRunning) RakSleep(30);

	std::cin.get();

	return 0;
}
