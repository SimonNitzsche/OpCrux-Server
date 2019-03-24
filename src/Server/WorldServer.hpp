#pragma once
#include "Interfaces/ILUServer.hpp"

class WorldServer : ILUServer {
public:
	WorldServer(int instanceID, int port);
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};