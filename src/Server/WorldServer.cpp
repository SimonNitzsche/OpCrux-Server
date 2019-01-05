#include "WorldServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>


#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/ESystem.hpp"

#include "Exceptions/ExNetException.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"

#include "Structs/Networking/General/StructPacketHeader.hpp"

#include "PacketFactory/General/GeneralPackets.hpp"
#include "PacketFactory/World/WorldPackets.hpp"
using namespace Exceptions;

WorldServer::WorldServer() : ILUServer() {
	// Initializes the RakPeerInterface used for the world server
	RakPeerInterface* rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Securiry
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND1", 8);

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)2001, 0);
	Logger::log("WRLD", "Starting World...");

	rakServer->SetMaximumIncomingConnections((unsigned short)2);

	// Check startup
	if (!rakServer->Startup(2, 30, &socketDescriptor, 1)) {
		std::cin.get();
		return;
	}

	Packet* packet;
	initDone = true;

	while (ServerInfo::bRunning) {
		RakSleep(1);
		while (packet = rakServer->Receive()) {
			try {
				handlePacket(rakServer, reinterpret_cast<LUPacket*>(packet));
			}
			catch (NetException::CorruptPacket e) {
				Logger::log("WRLD", "Received corrupt packet.", LogType::ERR);
				// TODO: Kick player.
				PacketFactory::General::doDisconnect(rakServer, packet->systemAddress, EDisconnectReason::UNKNOWN_SERVER_ERROR);
			}
		}
	}

	// QUIT
	Logger::log("WRLD", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
}

void WorldServer::handlePacket(RakPeerInterface* rakServer, LUPacket * packet) {
	RakNet::BitStream *data = new RakNet::BitStream(packet->getData(), packet->getLength(), false);
	LUPacketHeader packetHeader = packet->getHeader();

	switch (packetHeader.protocolID) {
	case ID_USER_PACKET_ENUM: {

		switch (static_cast<ERemoteConnection>(packetHeader.remoteType)) {
		case ERemoteConnection::GENERAL: {
			if (packetHeader.packetID == 0) {
				/// Do Handshake
				Logger::log("WRLD", "Handshaking with client...");

				PacketFactory::General::doHandshake(rakServer, packet->getSystemAddress(), false);
			}
			break;
		}
		case ERemoteConnection::SERVER: {
			switch (static_cast<EWorldPacketID>(packetHeader.packetID)) {
			case EWorldPacketID::CLIENT_CHARACTER_LIST_REQUEST: {
				PacketFactory::World::sendCharList(rakServer, packet->getSystemAddress());
				break;
			}
			case EWorldPacketID::CLIENT_CHARACTER_CREATE_REQUEST: {
				PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), EDisconnectReason::CHARACTER_CORRUPTION);
				break;
			}
			default:
				Logger::log("WRLD", "Received unknown packetID");
			}
		} break;
		default: {
			Logger::log("WRLD", "Recieved unknown packet type");
		}
		}

	} break;
	case ID_NEW_INCOMING_CONNECTION: {
		Logger::log("WRLD", "Recieving new Connection...");
		// TODO: Connect as Session
	} break;
	case ID_DISCONNECTION_NOTIFICATION: {
		Logger::log("WRLD", "User Disconnected from WORLD...");
		// TODO: Disconnect as Session
	} break;
	default: {
		Logger::log("WRLD", "Recieved unknown packet #" + (byte)packetHeader.packetID);
	}
	}

	// Deallocate the packet to conserve memory
	delete data;
	rakServer->DeallocatePacket(packet->getPacket());
}

WorldServer::~WorldServer() {

}