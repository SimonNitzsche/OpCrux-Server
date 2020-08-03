#ifndef _WORLD_INSTANCE_MANAGER_HPP__
#define _WORLD_INSTANCE_MANAGER_HPP__

#include "Server/WorldServer.hpp"
#include <stdlib.h>
#include <vector>

static class WorldInstanceManager {
private:
	std::vector<std::shared_ptr<WorldServer>> instances;
	std::uint32_t nextPortIndex = 0;

public:
	WorldServer * CreateInstance(std::uint16_t zoneID);
};

#endif // !_WORLD_INSTANCE_MANAGER_HPP__