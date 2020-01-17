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

#include "Configuration/ConfigurationManager.hpp"
#include "Database/Database.hpp"

using namespace std::chrono;

std::vector<ILUServer *> virtualServerInstances;

enum class SERVERMODE : uint8_t { STANDALONE, MASTER, WORLD, AUTH } MODE_SERVER;

GameCache::Interface::FDB::Connection Cache;
BridgeMasterServer* masterServerBridge;

int givenWorldID = 1303;


// Following Includes are for testing
#include "FileTypes/LUZFile/LUZone.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/WorldConfig.hpp"
#include "Entity/Components/StatsComponent.hpp"
#include "DataTypes/LDF.hpp"
#include "FileTypes/HKXFile/hkxFile.hpp"
#include <cop/cop.h>

int main(int argc, char* argv[]) {
	FileUtils::ChangeDirectory();
	std::string ipMaster = "127.0.0.1";
	//std::string ipMaster = "foxsog.com";

	std::string hf = "res/physics/env_nim_ag_puffytree.hkx";
	HKX::HKXFile hkx; hkx.Load(hf);

	Configuration::ConfigurationManager::Load();

	Logger::log("MAIN", "Connecting to database...");
	Database::Connect();
	//Database::DoATestQuery();

	Logger::log("MAIN", "Connecting game cache...");
	Cache.Connect("./res/cdclient.fdb");

	using namespace Entity;

	MODE_SERVER = SERVERMODE::STANDALONE;

	Logger::log("MAIN", "Booting up server instances...");

	//std::uintptr_t * c = CreateCOPScene();

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
