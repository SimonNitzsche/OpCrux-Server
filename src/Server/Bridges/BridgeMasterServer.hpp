#pragma once
#ifndef _BRIDGEMASTERSERVER_HPP
#define _BRIDGEMASTERSERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include "Common/CrossPlatform.hpp"
#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>


class BridgeMasterServer {
private:
	std::thread bridgeThread;
	RakPeerInterface* rakMasterClient;
	std::string masterServerIP;
	bool _connected = false;
	bool _listening = false;
protected:
	void ListenHandle();
public:
	BridgeMasterServer(std::string masterServerIP);
	BridgeMasterServer() {};
	~BridgeMasterServer();
	void Connect();
	void Listen();
};

#endif
