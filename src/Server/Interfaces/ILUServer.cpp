#include "ILUServer.hpp"

ILUServer::ILUServer(std::string masterServerIP) {
	masterServerBridge = new BridgeMasterServer(masterServerIP);
	masterServerBridge->Connect();
	initDone = true;
	masterServerBridge->Listen();
}