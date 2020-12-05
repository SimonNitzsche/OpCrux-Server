#include "BridgeMasterServer.hpp"

#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"

#include "Utils/Logger.hpp"
#include "Utils/PacketUtil.hpp"
#include "Utils/ServerInfo.hpp"
#include <Server\AuthServer.hpp>

#include "Server/WorldServer.hpp"

#include "PacketFactory/World/WorldPackets.hpp"

#include "Server/Manager/WorldInstanceManager.hpp"

using namespace Enums;

enum class SERVERMODE : uint8_t;
extern SERVERMODE MODE_SERVER;
extern AuthServer* authServer;

BridgeMasterServer::BridgeMasterServer(std::string masterServerIP) : masterServerIP (masterServerIP) {
	rakMasterClient = RakNetworkFactory::GetRakPeerInterface();
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
			uint8_t packetID;
			data->Read(packetID);

			switch (packetID) {
			case ID_USER_PACKET_ENUM: {
				uint16_t networkType;
				data->Read(networkType);
				uint32_t packetType;
				data->Read(packetType);
				uint8_t pad;
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
				case ERemoteConnection::SERVER: {
					switch (static_cast<EMasterPacketID>(packetType)) {
					case EMasterPacketID::MSG_MASTER_REQUEST_NEW_INSTANCE: {

						std::uint16_t port; data->Read(port);
						std::uint16_t zoneID; data->Read(zoneID);
						std::uint16_t instanceID; data->Read(instanceID);
						std::uint32_t cloneID; data->Read(cloneID);

						Logger::log("MasterBridge", "Starting new zone instance; zoneID=" + std::to_string(zoneID) + "; cloneID=" + std::to_string(instanceID) + "; cloneID=" + std::to_string(cloneID) + "; port=" + std::to_string(port));

						WorldInstanceManager::CreateInstance(zoneID, instanceID, cloneID, port);
						break;
					}
					case EMasterPacketID::MSG_MASTER_SELECT_WORLD_FOR_USER: {
						DataTypes::LWOOBJID objectID; data->Read(objectID);
						SystemAddress playerAddr; data->Read(playerAddr);
						std::string sessionKey = StringUtils::readStringFromBitStream<std::uint32_t>(data);
						std::uint16_t zoneID; data->Read(zoneID);
						std::uint16_t instanceID; data->Read(instanceID);
						std::uint32_t cloneID; data->Read(cloneID);
						SystemAddress instanceAddr; data->Read(instanceAddr);
						std::uint16_t port; data->Read(port);

						instanceAddr.port = port;
						authServer->DoPlayerLoginSuccess(playerAddr, instanceAddr, sessionKey);
						break;
					}
					case EMasterPacketID::MSG_IM_WORLD_CLIENT_TRANSFER_RESPONSE: {

						std::uint16_t sourcePort; data->Read(sourcePort);
						SystemAddress charIp; data->Read(charIp);
						SystemAddress processIp; data->Read(processIp);
						std::uint16_t destinationPort; data->Read(destinationPort);
						bool doAnouncment; data->Read(doAnouncment);

						WorldServer* remWs = WorldInstanceManager::GetInstance(sourcePort);


						PacketFactory::World::TransferToWorld(remWs->rakServer, charIp, const_cast<char*>(processIp.ToString(false)), destinationPort, doAnouncment);

						break;
					} 

					case EMasterPacketID::MSG_WORLD_CLIENT_LEVEL_LOAD_COMPLETE: {

						// WorldServer * remWs = WorldInstanceManager::GetInstance(sourcePort)

						// PacketFactory::World::LoadStaticZone(rakServer, &csFactory, luZone->zoneID, 0, 0, luZone->revisionChecksum, luZone->spawnPos.pos, 0);
					} break;
					case EMasterPacketID::MSG_MASTER_PLAYER_WORLD_AUTH_COMPLETED: {

						ClientSession clSession; clSession.Deserialize(data);

						WorldServer* remWs = WorldInstanceManager::GetInstance(clSession.connectedServerPort);
						remWs->FinishClientTransfer(clSession);

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
	Database::Disconnect(dbConn);
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
	dbConn = Database::Connect();
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

void BridgeMasterServer::ClientLoginAuth(SystemAddress systemAddress, int accountID) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	ClientSession clSession{};
	clSession.accountID = accountID;

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_IM_WORLD_CLIENT_LOGIN_REQUEST));
	RakNet::BitStream* packetPTR = packet.get();

	packet->Write(false); // Is World?

	StringUtils::writeStringToBitStream(packetPTR, std::string(systemAddress.ToString(true)));
	clSession.Serialize(packetPTR);

	Logger::log("Bridge", "Send Client logon");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void BridgeMasterServer::ClientLoginRespond(SystemAddress systemAddress, int accountID, int reason) {
	authServer->MasterClientAuthResponse(systemAddress, accountID, reason);
}

void BridgeMasterServer::ClientWorldAuth(SystemAddress systemAddress, ClientSession clSession) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_IM_WORLD_CLIENT_LOGIN_REQUEST));
	RakNet::BitStream* packetPTR = packet.get();

	packet->Write(true); // Is World?

	StringUtils::writeStringToBitStream(packetPTR, std::string(systemAddress.ToString(true)));
	clSession.Serialize(packetPTR);
	
	Logger::log("Bridge", "Send Client logon");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void BridgeMasterServer::ClientCharAuth(ClientSession * clientSession, std::uint16_t sourcePort, DataTypes::LWOOBJID charID) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	Str_DB_CharInfo charInfo = Database::GetChar(dbConn, charID & 0xFFFFFF);

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_WORLD_NOTIFY_CHAR_SELECT));

	packet->Write(sourcePort);
	packet->Write(clientSession->accountID);
	packet->Write(clientSession->systemAddress);
	clientSession->actorID = charID;
	packet->Write(charID);
	packet->Write(charInfo.lastWorld);
	packet->Write(charInfo.lastClone);

	RakNet::BitStream* packetPTR = packet.get();
	Logger::log("Bridge", "Send Char select");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void BridgeMasterServer::ClientDisconnect(SystemAddress systemAddress, WorldServer * instance) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_IM_WORLD_CLIENT_LOGOUT_NOTIFY));

	packet->Write(systemAddress);
	packet->Write(instance->GetPort());

	RakNet::BitStream * packetPTR = packet.get();
	Logger::log("Bridge", "Send Client disconnect");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void BridgeMasterServer::ClientRequestMapChange(ClientSession clSession, DataTypes::ZoneInfo zi) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_IM_WORLD_CLIENT_TRANSFER_REQUEST));
	RakNet::BitStream* packetPTR = packet.get();

	clSession.Serialize(packetPTR);
	packet->Write(zi);


	Logger::log("Bridge", "Send Map Change Request");
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void BridgeMasterServer::NotifyInstanceLoaded(std::uint16_t zoneID, std::uint16_t instanceID, std::uint32_t cloneID, SystemAddress systemAddress) {
	if (!_connected) {
		throw std::runtime_error("Not connected to Master Server.");
	}
	if (!_listening) {
		throw std::runtime_error("Not listening.");
	}

	auto packet = PacketUtils::initPacket(ERemoteConnection::MASTER, static_cast<uint32_t>(EMasterPacketID::MSG_IM_WORLD_LEVEL_LOADED_NOTIFY));

	packet->Write(zoneID);
	packet->Write(instanceID);
	packet->Write(cloneID);
	packet->Write(systemAddress);

	RakNet::BitStream* packetPTR = packet.get();
	this->rakMasterClient->Send(packetPTR, SYSTEM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

BridgeMasterServer::~BridgeMasterServer() {
	bridgeThread.~thread();
}
