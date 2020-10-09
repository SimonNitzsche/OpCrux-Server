#include "WorldInstanceManager.hpp"
#include "DataTypes/ZoneInfo.hpp"

#include <thread>
#include "Utils/Logger.hpp"
#include "Server/WorldServer.hpp"

// <port, instance>
std::unordered_map<std::uint16_t, WorldServer*> WorldInstanceManager::instances;

void WorldInstanceManager::CreateInstance(int zone, int instanceID, int cloneID, int port) {
	
	WorldServer* testWs;
	std::thread wT([](WorldServer* ws, std::uint16_t zoneID, std::uint16_t cloneID, std::uint32_t instanceID, std::uint16_t port) { ws = new WorldServer(zoneID, instanceID, cloneID, port); }, testWs, zone, cloneID, instanceID, port);
	wT.detach();
}

void WorldInstanceManager::AddWorldServer(std::uint16_t port, WorldServer* instance) {
	// Check we don't have the instance already and push back
	if(instances.find(port) == instances.end())
		instances.insert({ port, instance });
}

WorldServer* WorldInstanceManager::GetInstance(std::uint16_t port) {
	auto it = instances.find(port);
	if (it != instances.end()) return it->second;
	Logger::log("WIM", "Couldn't find instance with port " + std::to_string(port), LogType::WARN);
	return nullptr;
}