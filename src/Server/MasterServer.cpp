#include "MasterServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <iostream>

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>


#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/PacketUtil.hpp"

extern enum class SERVERMODE : uint8_t;

MasterServer::MasterServer() {
	// Initializes the RakPeerInterface used for the auth server
	rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Securiry
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND2", 8);

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)1000, 0);
	Logger::log("MASTER", "Starting Master...");

	rakServer->SetMaximumIncomingConnections((unsigned short)2);

	// Check startup
	if (!rakServer->Startup(2, 30, &socketDescriptor, 1)) {
		system("pause");
		return;
	}

	listenThread = std::thread([](MasterServer * srv) { srv->Listen(); }, this);
}

void MasterServer::Listen() {
	Packet* packet;
	if (!isDone) isDone = true;

	while (ServerInfo::bRunning) {
		RakSleep(1);
		while (packet = rakServer->Receive()) {
			//Logger::log("MASTER", "Packet received");
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
				case ERemoteConnection::MASTER: {
					switch (static_cast<EMasterPacketID>(packetType)) {
					case EMasterPacketID::MSG_MASTER_AUTHENTIFICATE_PROCESS: {
						struct {
							RakNet::RakString computerName;
							RakNet::RakString osName;
							int processID;
							SERVERMODE serverMode;
						} contentStruct;
						
						data->Read(contentStruct.computerName);
						data->Read(contentStruct.osName);
						data->Read(contentStruct.processID);
						data->Read(contentStruct.serverMode);

						std::string cn = contentStruct.computerName;
						std::string on = contentStruct.osName;

						int c = 3; // I personally use c=3 for breakpoints, so this line get's removed in the future.
					} break;
					}
				} break;

				default: {
					Logger::log("MASTER", "Recieved unknown packet type.");
				}
				}

			} break;
			case ID_NEW_INCOMING_CONNECTION: {
				Logger::log("MASTER", "Recieving new Connection...");
				auto cpacket = PacketUtils::initPacket(ERemoteConnection::GENERAL, static_cast<uint8_t>(EMasterPacketID::MSG_CLIENT_REQUEST_AUTHENTIFACTE_PROCESS));
				
				RakNet::BitStream * cpacketPTR = cpacket.get();
				rakServer->Send(cpacketPTR, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			} break;
			case ID_DISCONNECTION_NOTIFICATION: {
				Logger::log("MASTER", "User Disconnected from Master...");
				// TODO: Disconnect as Session
			} break;
			default: {
				Logger::log("MASTER", "Recieved unknown packet #" + (byte)packetID);
			}
			}

			// Deallocate the packet to conserve memory
			delete data;
			rakServer->DeallocatePacket(packet);
		}
	}

	// QUIT
	Logger::log("MASTER", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
}


MasterServer::~MasterServer() {
	listenThread.~thread();
}
