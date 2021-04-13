#include "UGCServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"

UGCServer::UGCServer() : ILUServer() {
	// Initializes the RakPeerInterface used for the auth server

	Packet* packet;
	initDone = true;

	while (ServerInfo::bRunning) {
		RakSleep(1);
	}

	// QUIT
	Logger::log("AUTH", "Recieved QUIT, shutting down...");

}

UGCServer::~UGCServer()
{
}
