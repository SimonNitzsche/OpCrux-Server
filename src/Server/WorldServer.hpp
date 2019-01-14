#ifndef __SERVER__WORLDSERVER_HPP__
#define __SERVER__WORLDSERVER_HPP__

#include "Interfaces/ILUServer.hpp"

class WorldServer : ILUServer {
public:
	WorldServer();
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};

#endif // !__SERVER__WORLDSERVER_HPP__
