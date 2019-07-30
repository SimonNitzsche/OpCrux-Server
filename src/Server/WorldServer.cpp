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
#include <fstream>
#include "Database/Database.hpp"
using namespace Exceptions;

WorldServer::WorldServer(int instanceID, int port) {
	// Preload
	std::string buf;
	std::ifstream file1("res/names/minifigname_first.txt");
	while (std::getline(file1, buf)) mf_FirstNames.push_back(buf);
	file1.close();
	std::ifstream file2("res/names/minifigname_middle.txt");
	while (std::getline(file2, buf)) mf_MiddleNames.push_back(buf);
	file2.close();
	std::ifstream file3("res/names/minifigname_last.txt");
	while (std::getline(file3, buf)) mf_LastNames.push_back(buf);
	file3.close();

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

				std::wstring customName = StringUtils::readWStringFromBitStream(data);
				std::string s_customName(customName.begin(), customName.end());

				unsigned long predef_0; data->Read(predef_0);
				unsigned long predef_1; data->Read(predef_1);
				unsigned long predef_2; data->Read(predef_2);

				std::stringstream namegen;
				namegen << mf_FirstNames[predef_0] << mf_MiddleNames[predef_1] << mf_LastNames[predef_2];
				std::string genname = namegen.str();

				unsigned char unknownA; data->Read(unknownA);
				unsigned long headColor; data->Read(headColor);
				unsigned long head;	data->Read(head);
				unsigned long chestColor; data->Read(chestColor);
				unsigned long chest; data->Read(chest);
				unsigned long legs; data->Read(legs);
				unsigned long hairStyle; data->Read(hairStyle);
				unsigned long hairColor; data->Read(hairColor);
				unsigned long leftHand; data->Read(leftHand);
				unsigned long rightHand; data->Read(rightHand);
				unsigned long eyebrowStyle; data->Read(eyebrowStyle);
				unsigned long eyesStyle; data->Read(eyesStyle);
				unsigned long mouthStyle; data->Read(mouthStyle);
				unsigned char unknownB; data->Read(unknownB);

				if (unknownA != 0 || unknownB != 0) {
					Logger::log("CHAR-CREATION", "unknownA: " + std::to_string(unknownA));
					Logger::log("CHAR-CREATION", "unknownB: " + std::to_string(unknownB));
				}
				//PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), EDisconnectReason::CHARACTER_CORRUPTION);
				Database::CreateNewChar(0, s_customName, genname, headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle);
				
				PacketFactory::World::sendCharList(rakServer, packet->getSystemAddress());
				
				break;
			}
			default:
				Logger::log("WRLD", "Received unknown packetID "+std::to_string(packetHeader.packetID));
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
