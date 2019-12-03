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
#include "Enums/ESystem.hpp"

#include "Exceptions/ExNetException.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"

#include "Structs/Networking/General/StructPacketHeader.hpp"

#include "PacketFactory/General/GeneralPackets.hpp"
#include "PacketFactory/Auth/AuthPackets.hpp"
using namespace Exceptions;

#include "Database/Database.hpp"

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
				Logger::log("AUTH", "Received corrupt packet.", LogType::ERR);
				// TODO: Kick player.
				PacketFactory::General::doDisconnect(rakServer, packet->systemAddress, EDisconnectReason::KICK);
			}
		}
	}

	// QUIT
	Logger::log("AUTH", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
	
}

void AuthServer::handlePacket(RakPeerInterface* rakServer, LUPacket * packet) {
	RakNet::BitStream *data = new RakNet::BitStream(packet->getData(), packet->getLength(), false);
	LUPacketHeader packetHeader = packet->getHeader();

	switch (packetHeader.protocolID) {
	case ID_USER_PACKET_ENUM: {

		switch (static_cast<ERemoteConnection>(packetHeader.remoteType)) {
		case ERemoteConnection::GENERAL: {
			if (packetHeader.packetID == 0) {
				/// Do Handshake
				Logger::log("AUTH", "Handshaking with client...");

				PacketFactory::General::doHandshake(rakServer, packet->getSystemAddress(), true);
			}
			break;
		}
		case ERemoteConnection::AUTH: {
			/// Do Login
			if (packetHeader.packetID == 0) {

				if (data->GetNumberOfUnreadBits() < 3944) throw new NetException::CorruptPacket();

				Logger::log("AUTH", "Login requested");
				// TODO: Handle Login
				std::u16string name = StringUtils::readBufferedWStringFromBitStream(data);
				std::u16string pswd = StringUtils::readBufferedWStringFromBitStream(data, 41);
				uint16_t COMLANG; data->Read(COMLANG);
				ESystem platform; data->Read(platform);
				std::u16string procMemInfo = StringUtils::readBufferedWStringFromBitStream(data, 256);
				std::u16string clientGPU = StringUtils::readBufferedWStringFromBitStream(data, 128);

				Logger::log("AUTH", "Requesting Login: " + std::string(name.begin(), name.end()) + " <-> " + std::string(pswd.length(), "*"[0]));

				bool authSuccess = Database::IsLoginCorrect((char16_t*)name.c_str(), (char16_t*)pswd.c_str());

				Database::DebugTest();

				if (authSuccess)
					PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::SUCCESS);
				else
					PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::INVALID_LOGIN);
			}
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
		Logger::log("AUTH", "Recieved unknown packet #" + (byte)packetHeader.packetID);
	}
	}

	// Deallocate the packet to conserve memory
	delete data;
	rakServer->DeallocatePacket(packet->getPacket());
}

AuthServer::~AuthServer() {

}
