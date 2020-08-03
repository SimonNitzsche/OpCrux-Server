#include <Server/Manager/WorldInstanceManager.hpp>

std::vector<WorldServer*> instances = {};

uint16_t GetNextPort() {
	return 2000 + instances.size() + 1;
}

WorldServer* CreateInstance(unsigned short zoneID, unsigned short port) {
	WorldServer* ws = new WorldServer(zoneID, instances.size() + 1, port);
	instances.push_back(ws);
	return ws;
}

void CloseInstance(WorldServer* server) {
	server->~WorldServer();
	delete server;
}

bool IsInstanceFull(WorldServer* server) {
	if (server->rakServer->NumberOfConnections() >= 32) { return true; }
	else { return false; }
}

uint16_t GetCharacterServer() {
	bool found = false;
	for (auto instance : instances) {
		if (instance->GetZoneID() == 0 && instance->rakServer->NumberOfConnections() != 32) {
			found = true;
			return instance->GetServerPort();
		}
	}
	if (!found) { WorldServer* NewServer = CreateInstance(0, 2002); return NewServer->GetServerPort();}
}

uint16_t GetWorldServer(int worldID) {
	bool found = false;
	for (auto instance : instances) {
		if (instance->GetZoneID() == worldID && instance->rakServer->NumberOfConnections() != 32) {
			found = true;
			return instance->GetServerPort();
		}
	}
	if (!found) { WorldServer* NewServer = CreateInstance(worldID, 2000 + instances.size() + 1); return NewServer->GetServerPort(); }
}