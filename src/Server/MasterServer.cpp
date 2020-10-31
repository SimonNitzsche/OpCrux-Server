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
#include "Utils/LDFUtils.hpp"
#include "Database/Database.hpp"
#include <Sessions\ClientSession.hpp>
#include "../libs/libbcrypt/include/bcrypt/BCrypt.hpp"
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
						Machine * machine = new Machine();
						machine->machineName = contentStruct.computerName;
						machine->machineOS = contentStruct.osName;
						machine->dottedIP = packet->systemAddress.ToString(false);
						
						MachineProcess * proc = new MachineProcess();
						proc->port = packet->systemAddress.port;
						proc->processID = contentStruct.processID;
						proc->server_mode = contentStruct.serverMode;
						proc->systemAddress = packet->systemAddress;
						machine->processes.push_back(proc);

						// Check if in list
						bool alreadyExists = false;
						for (Machine * m : connected_machines)
							if (m->machineName == machine->machineName)
								if (m->machineOS == machine->machineOS)
									if (m->dottedIP == machine->dottedIP) {
										alreadyExists = true;
										proc->machine = m;
										m->processes.push_back(proc); break;
									}

						if (!alreadyExists) {
							Machine * m = connected_machines.emplace_back(machine);
							for (MachineProcess * proc : m->processes) {
								proc->machine = m;
							}

							/*
								Allocate available ports
							*/
							std::uint16_t portMin = std::stoi(Configuration::ConfigurationManager::portsConf.GetStringVal("World", "PORTMIN"));
							std::uint16_t portMax = std::stoi(Configuration::ConfigurationManager::portsConf.GetStringVal("World", "PORTMAX"));
							for (std::uint16_t i = portMin; i <= portMax; ++i) {
								m->availablePorts.push(i);
							}
						}

					} break;

					case EMasterPacketID::MSG_IM_WORLD_CLIENT_LOGIN_REQUEST: {
						MachineProcess * mp = GetMachineProcess(packet);
						
						bool isWorldRequest;
						ClientSession clSessRemLocal;
						ClientSessionMR sessionMR;
						RakNet::RakString sysAddress;
						data->Read(isWorldRequest);
						data->Read(sysAddress);
						sessionMR.systemAddress.SetBinaryAddress(sysAddress);
						clSessRemLocal.Deserialize(data);
						sessionMR.process = mp;
						sessionMR.accountID = clSessRemLocal.accountID;
						sessionMR.connectedServerPort = clSessRemLocal.connectedServerPort;

						if (mp->machine == nullptr) return;

						Logger::log("MASTER", "Received player login request: ");
						Logger::log("MASTER", "\tIP: " + std::string(sessionMR.systemAddress.ToString(true)));
						Logger::log("MASTER", "\tAccountID: " + std::to_string(sessionMR.accountID));
						Logger::log("MASTER", "on " + mp->machine->machineName + " #" + std::to_string(mp->processID) + " port " + std::to_string(sessionMR.connectedServerPort));


						ClientSessionMR* sessionMRInList = nullptr;
						// Is the player already registered within the session list?
						bool playerRegistered = false;
						for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
							if ((*it)->accountID == sessionMR.accountID) {
								sessionMRInList = GetClientSessionMR(sessionMR.accountID);
								sessionMRInList->process = mp;
								playerRegistered = true;
								break;
							}
						}

						// It isn't registered already, add it
						if (!playerRegistered)
							sessionMRInList = connected_clients.emplace_back(new ClientSessionMR(sessionMR));


						sessionMRInList->sessionState = ClientSessionMRState::IN_TRANSFER_QUEUE;

						// Add Client
						bool isPending = false;
						if (!isWorldRequest) {

							static const char * key_charset { "0123456789AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz\\+*~#'!§$%&/()=?ß`´@€" };

							std::string outkey = std::string( 512, '\0' );

							// Give it a session key
							auto dist_kc512 = std::uniform_int_distribution{ {}, strlen(key_charset) - 1 };

							std::generate_n(outkey.begin(), 512, [&]() {return key_charset[dist_kc512(RandomUtil::GetEngine())]; });

							std::string finalkey_raw = outkey + sessionMRInList->systemAddress.ToString();

							sessionMRInList->sessionKey = BCrypt::generateHash(finalkey_raw);


							sessionMRInList->SetVar<std::int32_t>(u"targetZone", 0);

							auto charServer = GetHubCharServer();
							if (charServer != nullptr) {
								MovePlayerFromAuthToSession(sessionMRInList, charServer);
							}
							else {
								isPending = true;
							}
						}
						else {

							// Set new instance
							sessionMRInList->currentInstance = GetInstanceByMachineProcessAndPort(mp, clSessRemLocal.connectedServerPort);
							sessionMRInList->connectedServerPort = clSessRemLocal.connectedServerPort;

							bool keyValid = 0 == strcmp(sessionMRInList->sessionKey.c_str(), reinterpret_cast<const char*>(clSessRemLocal.sessionToken.c_str()));

							if (!keyValid) {
								// TODO: Key invalid;
								Logger::log("MASTER", std::string("Key invalid for ") + sessionMRInList->systemAddress.ToString(), LogType::WARN);
								break;
							}

							// Get last player location
							//Database::GetChar()

							if (sessionMRInList->objectID == 0ULL) {
								// We go to char
							}
							else {
								
								 // Tell Server to tell client we're ready to complete loading.
								sessionMRInList->sessionState = ClientSessionMRState::IN_WORLD_LOADING;
								ClientWorldTransferFinishedResponse(sessionMRInList);
							}

						}


						if (isPending) {
							if (!isWorldRequest) {
								RequestNewZoneInstance(0, 0);
							}
						}

					} break;

					case EMasterPacketID::MSG_IM_WORLD_CLIENT_LOGOUT_NOTIFY: {
						SystemAddress sysAddress;
						data->Read(sysAddress);
						std::uint16_t serverPortFromDisconnect;
						data->Read(serverPortFromDisconnect);


						// Remove client
						for (int i = 0; i < connected_clients.size(); ++i) {
							if (connected_clients[i]->systemAddress == sysAddress) {
								auto it = connected_clients.begin() + i;

								// Make sure currentInstance is the same instance as the one the user is leaving from
								// Otherwise the session could be gone before being on the new instance while transfer
								if ((*it)->currentInstance->port != serverPortFromDisconnect) break;
								if ((*it)->sessionState == ClientSessionMRState::IN_TRANSFER) break;

								Logger::log("MASTER", "User ended playsession for account " + std::to_string((*it)->accountID));
								delete (*it);
								connected_clients.erase(connected_clients.begin()+i);
								break;
							}
						}

					} break;

					case EMasterPacketID::MSG_IM_WORLD_LEVEL_LOADED_NOTIFY: {
						std::uint16_t zoneID; data->Read(zoneID);
						std::uint16_t instanceID; data->Read(instanceID);
						std::uint32_t cloneID; data->Read(cloneID);
						SystemAddress sysAddress; data->Read(sysAddress);

						for (auto it = pending_instances.begin(); it != pending_instances.end(); ++it) {
							if ((*it)->zoneID == zoneID && (*it)->instanceID == instanceID && (*it)->cloneID == cloneID) {
								(*it)->port = sysAddress.port;

								for (auto it2 = (*it)->process->instances.begin(); it2 != (*it)->process->instances.end(); ++it2) {
									if ((*it2) == (*it)) {
										(*it)->process->instances.erase(it2);
										break;
									}
								}

								(*it)->process->instances.push_back(available_instances.emplace_back(*it));
								pending_instances.erase(it);

								break;
							}
						}

						CheckTransferQueue(zoneID, instanceID, cloneID);

					} break;

					case EMasterPacketID::MSG_WORLD_NOTIFY_CHAR_SELECT: {
						MachineProcess* mp = GetMachineProcess(packet);

						std::uint16_t sourcePort; data->Read(sourcePort);
						std::uint32_t accountID; data->Read(accountID);
						SystemAddress sysAddress; data->Read(sysAddress);
						DataTypes::LWOOBJID charID; data->Read(charID);
						std::uint16_t lastZone; data->Read(lastZone);
						std::uint32_t lastClone; data->Read(lastClone);
						ClientSessionMR * mrClientSession = GetClientByAccountID(accountID);

						if (mrClientSession == nullptr) {
							Logger::log("MASTER", "Couldn't find player with accountID " + std::to_string(std::uint64_t(accountID)), LogType::ERR);
							break;
						}
						mrClientSession->objectID = charID;
						SystemAddress charIp = mrClientSession->systemAddress;


						std::uint16_t nextZoneID = lastZone;
						std::uint16_t nextCloneID = lastClone;
						bool ignoreSoftCap = false;

						if (nextZoneID == 0) nextZoneID = 1000;

						auto remoteNextInstance = SelectInstanceToJoin(nextZoneID, nextCloneID, ignoreSoftCap);

						// Check if we are already on the correct instance,
						// If we are we shouldn't do anything, as the client wouldn't
						// destruct himself and the other objects
						// causing a 0% loading issue
						if (mrClientSession->currentInstance == remoteNextInstance) break;

						if (remoteNextInstance != nullptr) {
							MovePlayerSessionToNewInstance(mrClientSession, remoteNextInstance);
						}
						else {
							mrClientSession->sessionState = ClientSessionMRState::IN_TRANSFER_QUEUE;
							mrClientSession->SetVar(u"targetZone", nextZoneID);
							mrClientSession->SetVar(u"targetClone", nextCloneID);
							RequestNewZoneInstance(nextZoneID, nextCloneID);
						}

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
			case ID_CONNECTION_LOST:
			case ID_DISCONNECTION_NOTIFICATION: {
				Logger::log("MASTER", "Server Disconnected from Master...");
				// TODO: Drop from server pool
			} break;
			default: {
				Logger::log("MASTER", "Recieved unknown packet #" + (byte)packetID);
			}
			}

			// Deallocate the packet to conserve memory
			delete data;
			rakServer->DeallocatePacket(packet);
		}
	
		// Other stuff

	}

	// QUIT
	Logger::log("MASTER", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
}

void MasterServer::CheckTransferQueue(std::uint16_t zoneID, std::uint16_t instanceID, std::uint32_t cloneID) {
	for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
		if ((*it)->sessionState == ClientSessionMRState::IN_TRANSFER_QUEUE) {
			if ((*it)->GetVar<std::int32_t>(u"targetZone") == zoneID && (*it)->GetVar<std::int32_t>(u"targetClone") == cloneID) {
				
				RemoteWorldInstance* rwi;
				rwi = SelectInstanceToJoin(zoneID, cloneID);

				if(rwi==nullptr) throw "Help!";

				// Are we still auth?
				if ((*it)->currentInstance == nullptr) {
					MovePlayerFromAuthToSession(*it, rwi);
				}
				// Otherwise move
				else {
					(*it)->currentInstance = GetInstanceByMachineProcessAndPort((*it)->process, (*it)->connectedServerPort);
					MovePlayerSessionToNewInstance(*it, rwi);
				}

			}
		}
	}
}

std::uint32_t MasterServer::GetPlayerCountOfInstance(RemoteWorldInstance* instance) {
	std::uint32_t counter = 0;
	for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
		if ((*it)->currentInstance != instance) continue;
		++counter;
	}
	return counter;
}

#include "GameCache/ZoneTable.hpp"

RemoteWorldInstance* MasterServer::SelectInstanceToJoin(std::uint16_t zoneID, std::uint32_t cloneID/* = 0*/, bool ignoreSoftCap/* = false*/) {
	Logger::log("MASTER", "Looking for instance to join; zoneID=" + std::to_string(zoneID) + "; cloneID=" + std::to_string(cloneID) + "; ignoreSoftCap=" + (ignoreSoftCap ? "true" : "false"));
	std::int32_t cap = 0;
	if (zoneID != 0) {
		if (ignoreSoftCap) {
			cap = CacheZoneTable::GetPopulationHardCap(zoneID);
		}
		else {
			cap = CacheZoneTable::GetPopulationSoftCap(zoneID);
		}
	}
	else {
		cap = 32000;
	}

	RemoteWorldInstance * bestInstance = nullptr;

	for (auto it = available_instances.begin(); it != available_instances.end(); ++it) {
		if ((*it)->zoneID != zoneID || (*it)->cloneID != cloneID) continue;
		std::uint32_t pop = GetPlayerCountOfInstance((*it));
		if (pop <= cap) {
			if (bestInstance == nullptr || GetPlayerCountOfInstance(bestInstance) > pop) {
				bestInstance = (*it);
			}
		}
	}

	return bestInstance;
}

void MasterServer::RequestNewZoneInstance(std::uint16_t zoneID, std::uint32_t cloneID) {
	
	// Check if we already have requested it.
	for (auto it = pending_instances.begin(); it != pending_instances.end(); ++it) {
		if ((*it)->zoneID == zoneID && (*it)->cloneID == cloneID)
			return;
	}

	Logger::log("MASTER", "Requesting new zone instance; zoneID=" + std::to_string(zoneID) + "; cloneID=" + std::to_string(cloneID));

	// Request it
	// - make instance id
	auto it = instanceID_counter.find(zoneID);
	if (it == instanceID_counter.end()) {
		instanceID_counter.insert({ zoneID, 0 });
		it = instanceID_counter.find(zoneID);
	}
	else {
		++it->second;
	}
	auto instanceID = it->second;

	std::float_t fLightestMachine = 0.0f;
	Machine* lightestMachine = nullptr;
	// - get lightest machine
	for (auto it2 = connected_machines.begin(); it2 != connected_machines.end(); ++it2) {
		
		std::float_t machineWeight = 0.0f;
		for (auto it3 = (*it2)->processes.begin(); it3 != (*it2)->processes.end(); ++it3) {
			std::float_t processWeight = 0.0f;
			for (auto it4 = (*it3)->instances.begin(); it4 != (*it3)->instances.end(); ++it4) {
				processWeight += CacheZoneTable::GetFZoneWeight((*it4)->zoneID);
			}
			machineWeight += processWeight;
		}

		if (lightestMachine == nullptr || machineWeight < fLightestMachine) {
			lightestMachine = (*it2);
			fLightestMachine = machineWeight;
		}
	}

	if (lightestMachine == nullptr) return;

	std::float_t fLightestMachineProcess = 0.0f;
	MachineProcess* lightestMachineProcess = nullptr;
	// - get lightest machine process
	for (auto it3 = lightestMachine->processes.begin(); it3 != lightestMachine->processes.end(); ++it3) {
		std::float_t processWeight = 0.0f;
		for (auto it4 = (*it3)->instances.begin(); it4 != (*it3)->instances.end(); ++it4) {
			processWeight += CacheZoneTable::GetFZoneWeight((*it4)->zoneID);
		}

		if (lightestMachineProcess == nullptr || processWeight < fLightestMachineProcess) {
			lightestMachineProcess = (*it3);
			fLightestMachineProcess = processWeight;
		}
	}

	if (lightestMachineProcess == nullptr) return;

	// make instance object
	RemoteWorldInstance * remInstance = new RemoteWorldInstance();
	remInstance->cloneID = cloneID;
	remInstance->instanceID = instanceID;
	remInstance->port = lightestMachine->availablePorts.front();
	lightestMachine->availablePorts.pop();
	remInstance->zoneID = zoneID;
	remInstance->process = lightestMachineProcess;

	// add it to pending list and process
	lightestMachineProcess->instances.push_back(pending_instances.emplace_back(remInstance));

	// tell the process to make new instance
	auto cpacket = PacketUtils::initPacket(ERemoteConnection::SERVER, static_cast<uint8_t>(EMasterPacketID::MSG_MASTER_REQUEST_NEW_INSTANCE));

	RakNet::BitStream* cpacketPTR = cpacket.get();
	cpacketPTR->Write(remInstance->port);
	cpacketPTR->Write(remInstance->zoneID);
	cpacketPTR->Write(remInstance->instanceID);
	cpacketPTR->Write(remInstance->cloneID);

	rakServer->Send(cpacketPTR, HIGH_PRIORITY, RELIABLE_ORDERED, 0, lightestMachineProcess->systemAddress, false);
}

ClientSessionMR* MasterServer::GetClientByObjectID(DataTypes::LWOOBJID objectID) {
	for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
		if ((*it)->objectID == objectID)
			return (*it);
	}

	Logger::log("MASTER", "Couldn't find player by objectID " + std::to_string(std::uint64_t(objectID)), LogType::WARN);
	return nullptr;
}

ClientSessionMR* MasterServer::GetClientByAccountID(std::uint32_t accountID) {
	for (auto it = connected_clients.begin(); it != connected_clients.end(); ++it) {
		if ((*it)->accountID == accountID)
			return (*it);
	}

	Logger::log("MASTER", "Couldn't find player by accountID " + std::to_string(std::uint64_t(accountID)), LogType::WARN);
	return nullptr;
}

RemoteWorldInstance * MasterServer::GetInstanceByMachineProcessAndPort(MachineProcess* machineProcess, std::uint16_t port) {
	for (int i = 0; i < machineProcess->instances.size(); ++i) {
		RemoteWorldInstance * rwi = machineProcess->instances.at(i);
		if (rwi->port != port) continue;
		return rwi;
	}
	
	return nullptr;
}

ClientSession MasterServer::ConvertSessionToCl(ClientSessionMR* sessionMR) {

	ClientSession clSession;
	clSession.accountID = sessionMR->accountID;
	clSession.actorID = sessionMR->objectID;
	clSession.connectedServerPort = sessionMR->connectedServerPort;
	clSession.currentZone.zoneID = sessionMR->currentInstance->zoneID;
	clSession.currentZone.zoneInstance = sessionMR->currentInstance->instanceID;
	clSession.currentZone.zoneClone = sessionMR->currentInstance->cloneID;
	clSession.systemAddress = sessionMR->systemAddress;
	return clSession;
}

void MasterServer::ClientWorldTransferFinishedResponse(ClientSessionMR* sessionMR) {
	auto cpacket = PacketUtils::initPacket(ERemoteConnection::SERVER, static_cast<uint8_t>(EMasterPacketID::MSG_MASTER_PLAYER_WORLD_AUTH_COMPLETED));

	RakNet::BitStream* cpacketPTR = cpacket.get();
	ConvertSessionToCl(sessionMR).Serialize(cpacketPTR);

	rakServer->Send(cpacketPTR, HIGH_PRIORITY, RELIABLE_ORDERED, 0, sessionMR->process->systemAddress, false);
}

MasterServer::~MasterServer() {
	listenThread.~thread();
}

RemoteWorldInstance* MasterServer::GetHubCharServer() {
	for (RemoteWorldInstance * instance : this->available_instances) {
		if (instance->zoneID == 0)
			return instance;
	}

	return nullptr;
}

void MasterServer::MovePlayerFromAuthToSession(ClientSessionMR * playerSession, RemoteWorldInstance * instance) {
	auto cpacket = PacketUtils::initPacket(ERemoteConnection::SERVER, static_cast<uint8_t>(EMasterPacketID::MSG_MASTER_SELECT_WORLD_FOR_USER));

	RakNet::BitStream* cpacketPTR = cpacket.get();
	cpacketPTR->Write(playerSession->objectID);
	cpacketPTR->Write(playerSession->systemAddress);
	StringUtils::writeStringToBitStream<uint32_t>(cpacketPTR, playerSession->sessionKey);
	cpacketPTR->Write(instance->zoneID);
	cpacketPTR->Write(instance->instanceID);
	cpacketPTR->Write(instance->cloneID);
	cpacketPTR->Write(instance->process->systemAddress);
	cpacketPTR->Write(instance->port);

	rakServer->Send(cpacketPTR, HIGH_PRIORITY, RELIABLE_ORDERED, 0, instance->process->systemAddress, false);

	playerSession->sessionState = ClientSessionMRState::IN_TRANSFER;
}

void MasterServer::MovePlayerSessionToNewInstance(ClientSessionMR * playerSession, RemoteWorldInstance * instance) {

	auto cpacket = PacketUtils::initPacket(ERemoteConnection::SERVER, static_cast<uint8_t>(EMasterPacketID::MSG_IM_WORLD_CLIENT_TRANSFER_RESPONSE));

	playerSession->sessionState = ClientSessionMRState::IN_TRANSFER;

	RakNet::BitStream* cpacketPTR = cpacket.get();
	cpacketPTR->Write(playerSession->currentInstance->port);
	cpacketPTR->Write(playerSession->systemAddress);
	cpacketPTR->Write(instance->process->systemAddress);
	cpacketPTR->Write(instance->port);
	cpacketPTR->Write(false);


	Logger::log("MASTER", "Sending " + std::to_string(std::uint64_t(playerSession->objectID)) + " to " + instance->process->systemAddress.ToString(false) + ":" + std::to_string(instance->port));
	rakServer->Send(cpacketPTR, HIGH_PRIORITY, RELIABLE_ORDERED, 0, playerSession->currentInstance->process->systemAddress, false);
}
