#include "BridgeMasterServer.hpp"

#include "../../Enums/EPackets.hpp"
#include "../../Enums/ERemoteConnection.hpp"

#include "../../Utils/Logger.hpp"

BridgeMasterServer::BridgeMasterServer(std::string masterServerIP) : masterServerIP (masterServerIP) {
	rakMasterClient = RakNetworkFactory::GetRakPeerInterface();
	Packet* msPacket;

	SocketDescriptor msSocketDesscriptor(0, 0);
	rakMasterClient->Startup(1, 30, &msSocketDesscriptor, 1);
	rakMasterClient->SetOccasionalPing(true);
}

void BridgeMasterServer::ListenHandle() {

}

void BridgeMasterServer::Connect() {
	if (_connected) {
		throw std::runtime_error("Already connected to Master Server.");
	}
	if (rakMasterClient->Connect(masterServerIP.c_str(), 1000, "3.25 ND2", 8)) {
		_connected = true;
	}
	else {
		throw std::runtime_error("Couldn't connect to Master Server.");
	}
}

void BridgeMasterServer::Listen() {
	if(!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (_listening) {
		throw std::runtime_error("Already listening to the Master Server.");
	}
	bridgeThread = std::thread([](BridgeMasterServer * srv) {
		srv->ListenHandle();
	}, this);
	bridgeThread.detach();
}

BridgeMasterServer::~BridgeMasterServer() {
	bridgeThread.~thread();
}
