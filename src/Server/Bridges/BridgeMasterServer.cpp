#include "BridgeMasterServer.hpp"

#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"

#include "Utils/Logger.hpp"
#include "Utils/PacketUtil.hpp"
#include "Utils/ServerInfo.hpp"

extern enum class SERVERMODE : uint8_t;
extern SERVERMODE MODE_SERVER;

BridgeMasterServer::BridgeMasterServer(std::string masterServerIP) : masterServerIP (masterServerIP) {
	rakMasterClient = RakNetworkFactory::GetRakPeerInterface();
	Packet* msPacket;

	SocketDescriptor msSocketDesscriptor(0, 0);
	rakMasterClient->Startup(1, 30, &msSocketDesscriptor, 1);
	rakMasterClient->SetOccasionalPing(true);
}

void BridgeMasterServer::ListenHandle() {
	_listening = true;
	Packet * packet;
	while (ServerInfo::bRunning) {
		RakSleep(1);
		while (packet = rakMasterClient->Receive()) {
			RakNet::BitStream *data = new RakNet::BitStream(packet->data, packet->length, false);
			unsigned char packetID;
			data->Read(packetID);

			switch (packetID) {
			case ID_USER_PACKET_ENUM: {
				unsigned short networkType;
				data->Read(networkType);
				unsigned long packetType;
				data->Read(packetType);
				unsigned char pad;
				data->Read(pad);

				switch (static_cast<ERemoteConnection>(networkType)) {
				case ERemoteConnection::GENERAL: {
					switch (static_cast<EMasterPacketID>(packetType)) {
					case EMasterPacketID::MSG_CLIENT_REQUEST_AUTHENTIFACTE_PROCESS: {
						SayHello();
						break;
					}
					default: {
						Logger::log("MasterBridge", "Received unknown packet.");
					}
					}
				} break;
				default: {
					Logger::log("MasterBridge", "Recieved unknown packet type.");
				}
				}

			} break;
			case ID_NEW_INCOMING_CONNECTION: {
				Logger::log("MasterBridge", "Recieving new Connection...");
				// TODO: Connect as Session
			} break;
			case ID_DISCONNECTION_NOTIFICATION: {
				Logger::log("MasterBridge", "User Disconnected from AUTH...");
				// TODO: Disconnect as Session
			} break;
			case ID_CONNECTION_REQUEST_ACCEPTED: {
				Logger::log("MasterBridge", "Connection to Master Server has been accepted!");
			} break;
			default: {
				Logger::log("MasterBridge", "Recieved unknown packet #" + (byte)packetID);
			}
			}

			// Deallocate the packet to conserve memory
			delete data;
			rakMasterClient->DeallocatePacket(packet);
		}
	}

	// QUIT
	Logger::log("AUTH", "Recieved QUIT, shutting down...");

	rakMasterClient->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakMasterClient);
}

void BridgeMasterServer::Connect() {
	if (_connected) {
		throw std::runtime_error("Already connected to Master Server.");
	}
	Logger::log("MasterBridge", "Connecting to Master Server...");
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

#include <iostream>
#include <ios>
#include <ostream>
#include <fstream>

void BridgeMasterServer::SayHello() {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_MASTER_AUTHENTIFICATE_PROCESS));

	packet->Write(RakNet::RakString(ServerInfo::getComputerName().c_str()));
	packet->Write(RakNet::RakString(ServerInfo::getOsName().c_str()));
	packet->Write(ServerInfo::processID);
	packet->Write(static_cast<uint8_t>(MODE_SERVER));

	RakNet::BitStream * packetPTR = packet.get();
	Logger::log("Bridge", "Send SayHello()");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

BridgeMasterServer::~BridgeMasterServer() {
	bridgeThread.~thread();
}
