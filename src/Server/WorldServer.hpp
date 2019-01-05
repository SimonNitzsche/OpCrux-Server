#pragma once
#include "Interfaces/ILUServer.hpp"

class WorldServer : ILUServer {
public:
	WorldServer();
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};