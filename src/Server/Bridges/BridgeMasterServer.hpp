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
#include "DataTypes/LWOOBJID.hpp"
#include "Sessions/ClientSession.hpp"
#include "DataTypes/ZoneInfo.hpp"

class WorldServer;

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
	void ClientLoginAuth(SystemAddress systemAddress, int accountID);
    void ClientLoginRespond(SystemAddress systemAddress, int accountID, int reason);
	void ClientWorldAuth(SystemAddress systemAddress, ClientSession clSession);
    void ClientCharAuth(ClientSession* clientSession, std::uint16_t sourcePort, DataTypes::LWOOBJID charID);
    void ClientDisconnect(SystemAddress systemAddress, WorldServer* instance);
    void ClientRequestMapChange(ClientSession clSession, DataTypes::ZoneInfo zi);
    void NotifyInstanceLoaded(std::uint16_t zoneID, std::uint16_t instanceID, std::uint32_t cloneID, SystemAddress systemAddress);
};

#endif // !__SERVER__BRIDGES__BRIDGEMASTERSERVER_HPP__
