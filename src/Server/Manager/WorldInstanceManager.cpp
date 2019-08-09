#include "WorldInstanceManager.hpp"
#include "DataTypes/ZoneInfo.hpp"

std::vector<std::shared_ptr<WorldServer>> WorldInstanceManager::instances;

WorldServer * CreateInstance(std::uint16_t zoneID) {
	//masterServerBridge.

	std::shared_ptr<WorldServer> server = std::make_shared<WorldServer>();
}