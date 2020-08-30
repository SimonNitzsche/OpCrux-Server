#include "AuthServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>

#include <array>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <string>
#include <memory>
#ifdef OPCRUX_PLATFORM_WIN32
#define popen _popen
#define pclose _pclose
#endif

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
	rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Securiry
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND1", 8);

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor(1001, 0);
	Logger::log("AUTH", "Starting Auth...");

	// Max Connections
	std::int32_t maxConnections = 128;
	rakServer->SetMaximumIncomingConnections(maxConnections);

	// Check startup
	if (!rakServer->Startup(maxConnections, 30, &socketDescriptor, 1)) {
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

void AuthServer::RequestMasterUserAuthConfirmation(SystemAddress systemAddress, std::uint64_t accountID) {
	masterServerBridge->ClientLoginAuth(systemAddress, accountID);
}

std::string AuthServer::MakeAccountAPICall(std::string scope, std::unordered_map<std::string, std::string> args) {
	std::pair<int, std::string> response;

	std::string cmd = "curl -X POST -F \"application-key=";
	cmd += Configuration::ConfigurationManager::dbConf.GetStringVal("ExtAccountService", "APPKEY");
	cmd += "\"";
	for (auto it = args.begin(); it != args.end(); ++it) {
		cmd += " -F \"";
		cmd += it->first;
		cmd += "=";
		cmd += it->second;
		cmd += "\"";
	}
	cmd += " \"" + Configuration::ConfigurationManager::dbConf.GetStringVal("ExtAccountService", "HOSTURL");
	cmd += "/auth\"";

	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
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

				bool authSuccess = 0;

				

				if (Configuration::ConfigurationManager::dbConf.GetStringVal("ExtAccountService", "ExtAccountService") == "FALSE") {
					bool authSuccess = Database::IsLoginCorrect((char16_t*)name.c_str(), (char16_t*)pswd.c_str());
					if (authSuccess) {
						std::uint64_t accountID = Database::GetAccountIDByClientName(std::string(name.begin(), name.end()));
						RequestMasterUserAuthConfirmation(packet->getSystemAddress(), accountID);
						//PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::SUCCESS);
					}
					else
						PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::INVALID_LOGIN);
				}
				else {
					bool authSuccess = MakeAccountAPICall("/auth", { {"username", std::string(name.begin(), name.end())}, {"password", std::string(pswd.begin(), pswd.end())} }) == "PASS";
					if (authSuccess) {
						std::uint64_t accountID = Database::GetAccountIDByClientName(std::string(name.begin(), name.end()));
						RequestMasterUserAuthConfirmation(packet->getSystemAddress(), accountID);
						//PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::SUCCESS);
					}
					else
						PacketFactory::Auth::doLoginResponse(rakServer, packet->getSystemAddress(), ELoginReturnCode::INVALID_LOGIN);
				}
				
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

void AuthServer::MasterClientAuthResponse(SystemAddress systemAddress, int accountID, int reason) {
	PacketFactory::Auth::doLoginResponse(this->rakServer, systemAddress, ELoginReturnCode(reason));
}

AuthServer::~AuthServer() {

}
