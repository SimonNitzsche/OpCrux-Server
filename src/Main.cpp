#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include <RakNet\RakSleep.h>

#include "Server\Bridges\BridgeMasterServer.hpp"
#include "Server\MasterServer.hpp"
#include "Server\AuthServer.hpp"
#include "Server\WorldServer.hpp"

std::vector<ILUServer *> virtualServerInstances;

MasterServer *mS;
BridgeMasterServer *masterServerBridge;

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

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		std::thread aM([]() { mS = new MasterServer(); });
		aM.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		masterServerBridge = new BridgeMasterServer(ipMaster);
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		std::thread aT([]() { while (mS == nullptr || !mS->isDone) RakSleep(30); new AuthServer(); });
		aT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {

	}

	while (true)RakSleep(30);

	std::system("pause");
}