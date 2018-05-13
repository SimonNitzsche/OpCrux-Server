#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "RakNet\RakSleep.h"

#include "Server\Bridges\BridgeMasterServer.hpp"
#include "Server\MasterServer.hpp"
#include "Server\AuthServer.hpp"
#include "Server\WorldServer.hpp"

std::vector<ILUServer *> virtualServerInstances;

MasterServer *mS;
BridgeMasterServer *masterServerBridge;
#include "DataTypes\AMF3.hpp"
int main(int argc, char* argv[]) {
	enum class SERVERMODE { STANDALONE, MASTER, WORLD, AUTH } MODE_SERVER;
	std::string ipMaster;

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
		
		ipMaster = ( (argc > i) ? argv[i + 1] : "127.0.0.1" );

		break;
	}
	std::thread aM([]() { mS = new MasterServer(); });
	aM.detach();

	masterServerBridge = new BridgeMasterServer(ipMaster);
	masterServerBridge->Connect();
	masterServerBridge->Listen();
	//masterServerBridge->SendTest();

	std::thread aT([]() { while (mS == nullptr || !mS->isDone) RakSleep(30); new AuthServer("127.0.0.1"); });
	aT.detach();

	// Test


	//unsigned char * data = new unsigned char [16] { 0x09, 0x01, 0x0f, 0x76, 0x69, 0x73, 0x69, 0x62, 0x6c, 0x65, 0x02, 0x01, 0x0a, 0x00, 0x00, 0x00 };
	
	unsigned char * data = new unsigned char[4]{ 0xf7, 0xbf, 0xbf, 0xbf };

	RakNet::BitStream *bs = new RakNet::BitStream(data, sizeof(data), false);
	//AMF3::Read(bs);

	U29 a = U29(bs);

	while (true)RakSleep(30);

	std::system("pause");
}