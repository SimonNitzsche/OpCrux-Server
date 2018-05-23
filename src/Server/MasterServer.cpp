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
					/// Do Handshake
					Logger::log("AUTH", "Handshaking with client...");

					RakNet::BitStream returnBS;
					// Head
					returnBS.Write(static_cast<byte>(ID_USER_PACKET_ENUM));
					returnBS.Write(ERemoteConnection::GENERAL);
					returnBS.Write(EServerPacketID::VERSION_CONFIRM);
					returnBS.Write(static_cast<byte>(0x00));
					//Data
					returnBS.Write(171022UL); // version
					returnBS.Write(0x93UL); // ???
					returnBS.Write(1UL); // connType
					returnBS.Write(GetCurrentProcessId());
					returnBS.Write(static_cast<unsigned short>(0xff));
					returnBS.Write(RakNet::RakString("127.0.0.1"), 264);

					rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				} break;

				default: {
					Logger::log("MASTER", "Recieved unknown packet");
				}
				}

			} break;
			case ID_NEW_INCOMING_CONNECTION: {
				Logger::log("MASTER", "Recieving new Connection...");
				// TODO: Connect as Session
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
