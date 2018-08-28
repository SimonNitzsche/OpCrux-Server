#pragma once
#ifndef _ILUSERVER_HPP
#define _ILUSERVER_HPP

#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <iostream>
#include "NetworkDataTypes/LUPacket.hpp"
#include "Server/Bridges/BridgeMasterServer.hpp"

enum class SERVERMODE : uint8_t;
extern SERVERMODE MODE_SERVER;
extern BridgeMasterServer * masterServerBridge;

class ILUServer {
public:
	bool initDone;

	ILUServer() {};
	~ILUServer() {};
};

#endif