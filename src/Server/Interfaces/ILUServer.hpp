#ifndef __SERVER__INTERFACES__ILUSERVER_HPP__
#define __SERVER__INTERFACES__ILUSERVER_HPP__

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

#endif // !__SERVER__INTERFACES__ILUSERVER_HPP__
