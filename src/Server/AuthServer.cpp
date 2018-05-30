#include "AuthServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>


#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"

AuthServer::AuthServer() : ILUServer() {
	// Initializes the RakPeerInterface used for the auth server
	RakPeerInterface* rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Securiry
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND1", 8);

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)1001, 0);
	Logger::log("AUTH", "Starting Auth...");

	rakServer->SetMaximumIncomingConnections((unsigned short)2);

	// Check startup
	if (!rakServer->Startup(2, 30, &socketDescriptor, 1)) {
		system("pause");
		return;
	}

	Packet* packet;
	initDone = true;

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
					returnBS.Write(ServerInfo::processID);
					returnBS.Write(static_cast<unsigned short>(0xff));
					returnBS.Write(RakNet::RakString("127.0.0.1"), 264);

					rakServer->Send(&returnBS, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				} break;
				case ERemoteConnection::AUTH: {
					/// Do Login
					Logger::log("AUTH", "Login requested");
					// TODO: Handle Login
				} break;
				default: {
					Logger::log("AUTH", "Recieved unknown packet");
				}
				}

			} break;
			case ID_NEW_INCOMING_CONNECTION: {
				Logger::log("AUTH", "Recieving new Connection...");
				// TODO: Connect as Session
			} break;
			case ID_DISCONNECTION_NOTIFICATION: {
				Logger::log("AUTH", "User Disconnected from AUTH...");
				// TODO: Disconnect as Session
			} break;
			default: {
				Logger::log("AUTH", "Recieved unknown packet #" + (byte)packetID);
			}
			}

			// Deallocate the packet to conserve memory
			delete data;
			rakServer->DeallocatePacket(packet);
		}
	}

	// QUIT
	Logger::log("AUTH", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
	
}

AuthServer::~AuthServer() {

}
