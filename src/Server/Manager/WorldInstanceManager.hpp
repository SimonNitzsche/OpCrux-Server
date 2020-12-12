#ifndef _WORLD_INSTANCE_MANAGER_HPP__
#define _WORLD_INSTANCE_MANAGER_HPP__

class WorldServer;

#include <cstdint>
#include <stdlib.h>
#include <unordered_map>

class WorldInstanceManager {
private:
	static std::unordered_map<std::uint16_t, WorldServer *> instances;
	std::uint32_t nextPortIndex = 0;

public:
	static void CreateInstance(int zone, int instanceID, int cloneID, int port);
	static void AddWorldServer(std::uint16_t port, WorldServer* instance);
	static WorldServer* GetInstance(std::uint16_t port);
};

#endif // !_WORLD_INSTANCE_MANAGER_HPP__