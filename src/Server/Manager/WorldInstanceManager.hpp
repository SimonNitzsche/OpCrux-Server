#ifndef _WORLD_INSTANCE_MANAGER_HPP__
#define _WORLD_INSTANCE_MANAGER_HPP__

#include "Server/WorldServer.hpp"
#include "DataTypes/ZoneInfo.hpp"
#include <Configuration/ConfPorts.hpp>
#include <Configuration/ConfigurationManager.hpp>

#include <stdlib.h>
#include <vector>

uint16_t GetNextPort();
WorldServer* CreateInstance(unsigned short zoneID, unsigned short port);
uint16_t GetWorldServer(int worldID);
uint16_t GetCharacterServer();
bool IsInstanceFull(WorldServer* server);
void CloseInstance(WorldServer* server);

#endif // !_WORLD_INSTANCE_MANAGER_HPP__