#ifndef __SERVER__WORLDSERVER_HPP__
#define __SERVER__WORLDSERVER_HPP__

#include "Interfaces/ILUServer.hpp"
#include "Sessions/SessionManager.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"
class ObjectsManager;
#include "Entity/ObjectsManager.hpp"

#include "Misc/LWOTimer.hpp"

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
	RakPeerInterface* rakServer;
	FileTypes::LUZ::LUZone * luZone;
	unsigned long long spawnedObjectIDCounter=0;
	LWOTimer timer;
public:
	WorldServer(int zone, int instanceID, int port);
	void GameLoopThread();
	void GamePhysicsThread();
	void handlePacket(RakPeerInterface * rakServer, LUPacket * packet);
	~WorldServer();
};

#endif // !__SERVER__WORLDSERVER_HPP__
