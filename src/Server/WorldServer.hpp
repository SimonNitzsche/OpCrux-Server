#ifndef __SERVER__WORLDSERVER_HPP__
#define __SERVER__WORLDSERVER_HPP__

#include "Interfaces/ILUServer.hpp"
#include "Sessions/SessionManager.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
class ObjectsManager;
#include "Entity/ObjectsManager.hpp"

class WorldServer : ILUServer {
private:
	std::vector<std::string> mf_FirstNames = {};
	std::vector<std::string> mf_MiddleNames = {};
	std::vector<std::string> mf_LastNames = {};
public:
	SessionManager sessionManager;
	ReplicaManager * replicaManager = nullptr;
	NetworkIDManager * networkIdManager = nullptr;
	ObjectsManager * objectsManager = nullptr;
	FileTypes::LUZ::LUZone * luZone;
public:
	WorldServer(int zone, int instanceID, int port);
	void GameLoopThread();
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};

#endif // !__SERVER__WORLDSERVER_HPP__
