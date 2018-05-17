#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Common/CrossPlatform.hpp"

#include <RakNet/RakSleep.h>

#include "Server/MasterServer.hpp"
#include "Server/AuthServer.hpp"
#include "Server/WorldServer.hpp"
#include "Utils/ServerInfo.hpp"
#include "Server/Bridges/BridgeMasterServer.hpp"

std::vector<ILUServer *> virtualServerInstances;

MasterServer *mS;
BridgeMasterServer *masterServerBridge;
#include "DataTypes/AMF3.hpp"
int main(int argc, char* argv[]) {
	enum class SERVERMODE { STANDALONE, MASTER, WORLD, AUTH } MODE_SERVER;
	std::string ipMaster = "127.0.0.1";

	MODE_SERVER = SERVERMODE::STANDALONE;
	std::system("title LUReborn Server 3.0 (Standalone)");
	for (int i = 0; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (arg == "--master") {
			//MODE_SERVER = SERVER_MODE::MASTER;
			std::system("title LUReborn Server 3.0 (Master only)");
		}
		if (arg == "--world") {
			//MODE_SERVER = SERVER_MODE::WORLD;
			std::system("title LUReborn Server 3.0 (World only)");
		}
		if (arg == "--auth") {
			//MODE_SERVER = SERVER_MODE::AUTH;
			std::system("title LUReborn Server 3.0 (Auth only)");
		}
		else {
			continue;
		}

		if (argc >= 3) {
			ipMaster = argv[i + 1]; //Configfile?
		}

		break;
	}

	ServerInfo::init();

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		MasterServer * mS;
		std::thread mT([](MasterServer * ms) { ms = new MasterServer(); }, mS);
		mT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		BridgeMasterServer* masterServerBridge = new BridgeMasterServer(ipMaster);
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		AuthServer * aS;
		std::thread aT([](AuthServer * as) { as = new AuthServer(); }, aS);
		aT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {
		//std::thread wT([]() { new WorldServer(); });
		//wT.detach();
	}

	while (ServerInfo::bRunning) RakSleep(30);

	std::system("pause");
}
