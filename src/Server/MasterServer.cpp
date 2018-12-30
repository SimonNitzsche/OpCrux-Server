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
using namespace Enums;

enum class SERVERMODE : uint8_t;

MasterServer::MasterServer() {
	// Initializes the RakPeerInterface used for the auth server
	rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Security
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND2", 8);

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)1000, 0);
	Logger::log("MASTER", "Starting Master...");

	rakServer->SetMaximumIncomingConnections((unsigned short)2);

	// Check startup
	if (!rakServer->Startup(2, 30, &socketDescriptor, 1)) {
		std::cin.get();
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
				case ERemoteConnection::MASTER: {
					switch (static_cast<EMasterPacketID>(packetType)) {
					case EMasterPacketID::MSG_MASTER_AUTHENTIFICATE_PROCESS: {
						// Read
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

						
						// Log
						Logger::log("MASTER", "Received Hello from Server ("+ std::string(packet->systemAddress.ToString()) +"):");
						Logger::log("MASTER", std::string("\tComputerName: " + contentStruct.computerName));
						Logger::log("MASTER", std::string("\tOperationSystem: " + contentStruct.osName));
						Logger::log("MASTER", std::string("\tProcessID: " + std::to_string(contentStruct.processID)));
						Logger::log("MASTER", std::string("\tServerMode: " + std::to_string((int)contentStruct.serverMode)));

						// Create
						Machine machine = Machine();
						machine.machineName = contentStruct.computerName;
						machine.machineOS = contentStruct.osName;
						machine.dottedIP = packet->systemAddress.ToString(false);
						
						MachineProcess proc;
						proc.port = packet->systemAddress.port;
						proc.processID = contentStruct.processID;
						proc.server_mode = contentStruct.serverMode;
						machine.processes.push_back(proc);

						// Check if in list
						bool alreadyExists = false;
						for (Machine m : connected_machines)
							if (m.machineName == machine.machineName)
								if (m.machineOS == machine.machineOS)
									if (m.dottedIP == machine.dottedIP)
										if (alreadyExists = true)
											{ m.processes.push_back(proc); break; }

						if (!alreadyExists)
							connected_machines.push_back(machine);

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
