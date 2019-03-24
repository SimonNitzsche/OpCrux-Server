#ifndef __SERVER__BRIDGES__BRIDGEMASTERSERVER_HPP__
#define __SERVER__BRIDGES__BRIDGEMASTERSERVER_HPP__

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
	void SayHello();
	void ChooseWorldServer();
};

#endif // !__SERVER__BRIDGES__BRIDGEMASTERSERVER_HPP__
