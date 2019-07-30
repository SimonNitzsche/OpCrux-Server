#ifndef __SERVER__WORLDSERVER_HPP__
#define __SERVER__WORLDSERVER_HPP__

#include "Interfaces/ILUServer.hpp"

class WorldServer : ILUServer {
private:
	std::vector<std::string> mf_FirstNames = {};
	std::vector<std::string> mf_MiddleNames = {};
	std::vector<std::string> mf_LastNames = {};
public:
	WorldServer(int instanceID, int port);
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};

#endif // !__SERVER__WORLDSERVER_HPP__
