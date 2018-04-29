#pragma once
#include "Interfaces/ILUServer.hpp"

class WorldServer : ILUServer {
public:
	WorldServer(std::string masterServerIP);
	~WorldServer();
};