#include "WorldServer.hpp"

#include "Common/CrossPlatform.hpp"
#include <stdio.h>

#include <RakNet/BitStream.h>
#include <RakNet/MessageIdentifiers.h>
#include <RakNet/RakPeerInterface.h>
#include <RakNet/RakNetworkFactory.h>
#include <RakNet/RakSleep.h>
#include <RakNet/ReplicaManager.h>
#include <RakNet/NetworkIDManager.h>

#include <bullet3-2.89/src/btBulletDynamicsCommon.h>

#include "Enums/EPackets.hpp"
#include "Enums/ERemoteConnection.hpp"
#include "Enums/ESystem.hpp"
#include "Enums/EFriendRequestCodes.hpp"

#include "Exceptions/ExNetException.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"

#include "Structs/Networking/General/StructPacketHeader.hpp"
#include "Server/Manager/WorldInstanceManager.hpp"

#include "PacketFactory/General/GeneralPackets.hpp"
#include "PacketFactory/Chat/ChatPackets.hpp"
#include "PacketFactory/World/WorldPackets.hpp"
#include <fstream>
#include "Database/Database.hpp"

#include "DataTypes/LWOOBJID.hpp"

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/SpawnerComponent.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"
#include "Entity/Components/DestructibleComponent.hpp"
#include "Entity/Components/InventoryComponent.hpp"

#include "Entity/GameObject.hpp"

#include "GameCache/ZoneTable.hpp"
#include "GameCache/Objects.hpp"

#include "Utils/LDFUtils.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"

#include "Entity/GameMessages.hpp"
#include "Entity/Components/ModuleAssemblyComponent.hpp"
#include "Entity/Components/PossessableComponent.hpp"
#include "Entity/GameMessages/NotifyRacingClient.hpp"
#include "Entity/Components/RacingControlComponent.hpp"
#include "Entity/Components/MinigameComponent.hpp"
#include <Entity\Components\ScriptComponent.hpp>

#include "../../libs/recastnavigation/Recast/Include/Recast.h"

using namespace Exceptions;

extern BridgeMasterServer* masterServerBridge;
extern std::vector<ILUServer*> virtualServerInstances;

std::uint32_t WorldServer::GetCinematicInfo(std::u16string name) {
	if (!luZone || !luZone->_isFileLoaded()) return 0;
	auto pathIt = luZone->paths.find(name);

	if (pathIt == luZone->paths.end()) return 0;
	if (pathIt->second->pathType != FileTypes::LUZ::LUZonePathType::Camera) return 0;
	auto camPath = reinterpret_cast<FileTypes::LUZ::LUZonePathCamera*>(pathIt->second);
	
	std::uint32_t totalTime = 0;
	for (auto wp : camPath->waypoints) {
		totalTime += std::int32_t((wp->time) * 1000);
	}

	if (camPath->nextPath.length != 0) {
		totalTime += GetCinematicInfo(camPath->nextPath.ToString());
	}

    return totalTime;
}

WorldServer::WorldServer(int zone, int instanceID, int cloneID, int port) : m_port(port) {
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

	sessionManager = SessionManager();

	// Initializes the RakPeerInterface used for the world server
	rakServer = RakNetworkFactory::GetRakPeerInterface();

	// Initializes Securiry
	// TODO: Init Security
	rakServer->SetIncomingPassword("3.25 ND1", 8);

	dbConnection = Database::Connect();

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)port, 0);
	Logger::log("WRLD", "Starting world on port "+ std::to_string(port) + "...");

	short maxPlayers = 120;

	rakServer->SetMaximumIncomingConnections(maxPlayers);

	replicaManager = new ReplicaManager();
	rakServer->AttachPlugin(replicaManager);
	replicaManager->SetDefaultScope(true);
	networkIdManager = new NetworkIDManager();
	rakServer->SetNetworkIDManager(networkIdManager);

	replicaManager->SetAutoParticipateNewConnections(true);
	replicaManager->SetAutoSerializeInScope(true);

	networkIdManager->SetIsNetworkIDAuthority(true);

	this->objectsManager = new ObjectsManager(this);

	bool useDebugRenderer = false;
	if (useDebugRenderer) {
		//debugRenderer = new DebugRenderer();
		//debugRenderer->AssignZoneInstance(this);
	}

	// Check startup
	if (!rakServer->Startup(maxPlayers, 30, &socketDescriptor, 1)) {
		std::cin.get();
		return;
	}

	/*
		LOAD WORLD STUFF. Zone 0 is char selection only, so we don't need to load world.
	*/

	WorldInstanceManager::AddWorldServer(m_port, this);

	if (zone != 0) {
		/// collision configuration contains default setup for memory, collision setup.
		/// Advanced users can create their own configuration.
		collisionConfiguration = new btDefaultCollisionConfiguration();

		/// use the default collision dispatcher. For parallel processing you can use a diffent
		/// dispatcher(see Extras / BulletMultiThreaded)
		collisionDispatcher = new btCollisionDispatcher(collisionConfiguration);

		/// btDbvtBroadphase is a good general purpose broadphase. You can also try out
		/// btAxis3Sweep.
		overlappingPairCache = new btDbvtBroadphase();

		/// the default constraint solver. For parallel processing you can use a different solver
		/// (see Extras / BulletMultiThreaded)
		constraintSolver = new btSequentialImpulseConstraintSolver();

		/// Create physic world
		dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher, overlappingPairCache, constraintSolver, collisionConfiguration);

		// Set gravity
		dynamicsWorld->setGravity(btVector3(0, -CacheWorldConfig::GetPEGravityValue(), 0));

		// Get zone file
		std::string zoneName = CacheZoneTable::GetZoneName(zone);

		// Load Zone
		Logger::log("WRLD", "Loading Zone: " + zoneName);
		luZone = new FileTypes::LUZ::LUZone("res/maps/" + zoneName);
		if (!luZone->_isFileLoaded()) {
			delete collisionConfiguration;
			delete collisionDispatcher;
			delete overlappingPairCache;
			delete constraintSolver;
			delete dynamicsWorld;
			delete luZone;

			// TODO: Tell MS that loading failed

			return;
		}
		Logger::log("WRLD", "Sucessfully loaded zone.");

		if (luZone->zoneID != zone) {
			Logger::log("WRLD", "Invalid zoneID within LUZ file, correcting...", LogType::UNEXPECTED);
			luZone->zoneID = zone;
		}

		std::int32_t zoneControlLOT = CacheZoneTable::GetZoneControlTemplate(zone);
		if (zoneControlLOT == 0) zoneControlLOT = 2365;
		this->zoneControlObject = new Entity::GameObject(this, zoneControlLOT);
		this->zoneControlObject->SetObjectID(0x3FFFFFFFFFFE);
		std::int32_t zoneScriptID = CacheZoneTable::GetScriptID(zone);
		if (zoneScriptID != -1)
			this->zoneControlObject->AddComponent<ScriptComponent>(zoneScriptID);
		objectsManager->RegisterObject(this->zoneControlObject);
		this->zoneControlObject->Finish();

		// TODO: Move this to somewhere else
		for (const auto& scene : luZone->scenes) {
			for (auto objT : scene.scene.objectsChunk.objects) {

				// If we are the same LOT as zoneObject, continue
				if (*objT.LOT == this->zoneControlObject->GetLOT()) continue;

				auto* go = new Entity::GameObject(this, *objT.LOT);
				go->SetObjectID(DataTypes::LWOOBJID(/*(1ULL << 45)*/ 70368744177664ULL | *objT.objectID));
				LDFCollection ldfCollection = LDFUtils::ParseCollectionFromWString(objT.config.ToString());

				// If Spawner
				auto* spawnerComp = dynamic_cast<SpawnerComponent*>(go->GetComponentByType(10));
				if (spawnerComp != nullptr) {
					spawnerComp->originPos = objT.spawnPos->pos;
					spawnerComp->originRot = objT.spawnPos->rot;
				}
				else {
					go->SetPosition(objT.spawnPos->pos);
					go->SetRotation(objT.spawnPos->rot);
				}
				go->isSerializable = false;

				go->SetScale(*objT.scale);

				go->PopulateFromLDF(&ldfCollection);

				objectsManager->RegisterObject(go);
			}
		}

		// Path's Network Spawner
		for (const auto& pathBase : luZone->paths) {
			if (pathBase.second->pathType == FileTypes::LUZ::LUZonePathType::Spawner) {
				auto spawnerPath = reinterpret_cast<FileTypes::LUZ::LUZonePathSpawner*>(pathBase.second);
				WorldServer* Instance = this;

				// If we are the same LOT as zoneObject, continue
				if (spawnerPath->spawnedLOT == Instance->zoneControlObject->GetLOT()) continue;

				auto* spawner = new Entity::GameObject(this, 176);
				spawner->SetObjectID(spawnerPath->spawnerObjectID);

				LDFCollection ldfCollectionSpawner = {
					LDF_COLLECTION_INIT_ENTRY(u"respawn", spawnerPath->respawnTime),
					LDF_COLLECTION_INIT_ENTRY(u"no_auto_spawn", !spawnerPath->activateSpawnerNetworkOnLoad),
					LDF_COLLECTION_INIT_ENTRY(u"max_to_spawn", spawnerPath->maxToSpawn),
					LDF_COLLECTION_INIT_ENTRY(u"number_to_maintain", spawnerPath->numberToMaintain),
					LDF_COLLECTION_INIT_ENTRY(u"spawner_name", spawnerPath->pathName.ToString()),
					LDF_COLLECTION_INIT_ENTRY(u"is_network_spawner", true),
					LDF_COLLECTION_INIT_ENTRY(u"spawntemplate", std::int32_t(spawnerPath->spawnedLOT))
				};

				spawner->PopulateFromLDF(&ldfCollectionSpawner);
				objectsManager->RegisterObject(spawner);
			}
		}


		// Finish objects
		for (auto go : this->objectsManager->GetObjects()) {
			go->Finish();
		}
	}
	else {
		Logger::log("WORLD", "Loaded char server successfully");
	}

	virtualServerInstances.push_back(this);

	Packet* packet;
	initDone = true;

	Logger::log("WRLD", "Finished loading and initializing of the instance.", LogType::PASSED);

	std::thread glT([=]() { GameLoopThread(); });
	glT.detach();

	std::thread gpT([=]() { GamePhysicsThread(); });
	gpT.detach();

	// TODO: Tell MS that instance has loaded
	SystemAddress addr;
	addr.SetBinaryAddress(ServerInfo::GetAuthIP().c_str());
	addr.port = port;
	masterServerBridge->NotifyInstanceLoaded(zone, instanceID, cloneID, addr);

	while (ServerInfo::bRunning) {
		RakSleep(16);
		/*if (debugRenderer != nullptr) {
			debugRenderer->Paint();
		}*/
		try {
			// RakNet likes to crash in debug mode.
			// Skip packet if unable tro read
			m_lock.lock();
			packet = rakServer->Receive();
			m_lock.unlock();
		}
		catch (...) {
			continue;
		}

		if (packet == nullptr) continue;

		try {
			m_lock.lock();
			handlePacket(rakServer, reinterpret_cast<LUPacket*>(packet));
			m_lock.unlock();
		}
		catch (NetException::CorruptPacket e) {
			Logger::log("WRLD", "Received corrupt packet.", LogType::ERR);
			PacketFactory::General::doDisconnect(rakServer, packet->systemAddress, EDisconnectReason::UNKNOWN_SERVER_ERROR);
		}
		/*catch (std::runtime_error * e) {
			Logger::log("WRLD", "[CRASH] " + std::string(e->what()), LogType::ERR);
			throw std::runtime_error(*e);
		}*/
	}

	// QUIT
	Logger::log("WRLD", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
}

void WorldServer::GameLoopThread() {
	while (true) {
		m_lock.lock();
		objectsManager->OnUpdate();
		timer.Update();
		//dynamicsWorld->stepSimulation(0.0166667f, 10);
		objectsManager->OnPhysicsUpdate();
		m_lock.unlock();
		RakSleep(30);
	}
}

void WorldServer::DebugRendererThread() {
	while (ServerInfo::bRunning) {
		
		RakSleep(30);
	}
}

void WorldServer::GamePhysicsThread() {
	return;
	while (ServerInfo::bRunning) {
		m_lock.lock();
		
		m_lock.unlock();
		RakSleep(300);
	}
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
		case ERemoteConnection::CHAT: {
			switch (EChatPacketID(packetHeader.packetID)) {
			case EChatPacketID::ADD_FRIEND_REQUEST: {
				uint64_t unknown;
				data->Read<uint64_t>(unknown);
				std::u16string PersonToBeAddedAsFriend = StringUtils::readBufferedWStringFromBitStream(data);
				uint8_t isBestFriendRequest;
				data->Read<uint8_t>(isBestFriendRequest);
				
			}
			case EChatPacketID::ADD_FRIEND_RESPONSE: {
				uint64_t unknown;
				data->Read<uint64_t>(unknown);
				Enums::EFriendRequestCodes Status;
				data->Read<Enums::EFriendRequestCodes>(Status);
				std::u16string PersonToBeAddedAsFriend = StringUtils::readBufferedWStringFromBitStream(data);
			}
			case EChatPacketID::REMOVE_FRIEND: {
				uint64_t unknown;
				data->Read<uint64_t>(unknown);
				std::u16string FriendToBeRemoved = StringUtils::readBufferedWStringFromBitStream(data);
			}
			case EChatPacketID::TEAM_INVITE: {
				uint64_t unknown;
				data->Read<uint64_t>(unknown);
				std::u16string PersonInvited = StringUtils::readBufferedWStringFromBitStream(data);
			}
			case EChatPacketID::TEAM_INVITE_RESPONSE: {
				uint64_t unknown;
				data->Read<uint64_t>(unknown);
				uint8_t IsInviteDenied;
				data->Read<uint8_t>(IsInviteDenied);
				uint64_t InvitersOBJID;
				data->Read<uint64_t>(InvitersOBJID);
			}
			}
		}
		case ERemoteConnection::SERVER: {
			// TODO: Check if client is authenticated session.
			ClientSession * clientSession = nullptr;
			if (static_cast<EWorldPacketID>(packetHeader.packetID) != EWorldPacketID::CLIENT_VALIDATION) { // Whitelist CLIENT_VALIDATION
				clientSession = sessionManager.GetSession(packet->getSystemAddress());
				// Something went wrong
				if (clientSession == nullptr) {
					PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), Enums::EDisconnectReason::CHARACTER_NOT_FOUND);
					return;
				}
			}

			switch (EWorldPacketID(packetHeader.packetID)) {
			case EWorldPacketID::CLIENT_VALIDATION: {
				std::u16string wClientName = StringUtils::readBufferedWStringFromBitStream(data);
				std::u16string wClientKey = StringUtils::readBufferedWStringFromBitStream(data);
				std::string sClientKey = std::string((const char*)wClientKey.c_str());
				std::string sClientFDBChecksum = StringUtils::readBufferedStringFromBitStream(data);
				ClientSession csFactory{};
				csFactory.accountID = Database::GetAccountIDByClientName(GetDBConnection(), std::string(wClientName.begin(), wClientName.end()));
				csFactory.sessionToken = wClientKey;
				csFactory.systemAddress = packet->getSystemAddress();
				csFactory.connectedServerPort = this->m_port;

				sessionManager.AddSession(csFactory);

				masterServerBridge->ClientWorldAuth(packet->getSystemAddress(), csFactory);

				break;
			}
			case EWorldPacketID::CLIENT_CHARACTER_LIST_REQUEST: {
				PacketFactory::World::sendCharList(this, clientSession);
				break;
			}
			case EWorldPacketID::CLIENT_CHARACTER_CREATE_REQUEST: {

				std::u16string customName = StringUtils::readBufferedWStringFromBitStream(data);
				std::string s_customName(customName.begin(), customName.end());

				std::uint32_t predef_0; data->Read(predef_0);
				std::uint32_t predef_1; data->Read(predef_1);
				std::uint32_t predef_2; data->Read(predef_2);

				std::stringstream namegen;
				namegen << mf_FirstNames[predef_0] << mf_MiddleNames[predef_1] << mf_LastNames[predef_2];
				std::string genname = namegen.str();

				std::uint8_t unknownA; data->Read(unknownA);
				std::uint32_t headColor; data->Read(headColor);
				std::uint32_t head;	data->Read(head);
				std::uint32_t chestColor; data->Read(chestColor);
				std::uint32_t chest; data->Read(chest);
				std::uint32_t legs; data->Read(legs);
				std::uint32_t hairStyle; data->Read(hairStyle);
				std::uint32_t hairColor; data->Read(hairColor);
				std::uint32_t leftHand; data->Read(leftHand);
				std::uint32_t rightHand; data->Read(rightHand);
				std::uint32_t eyebrowStyle; data->Read(eyebrowStyle);
				std::uint32_t eyesStyle; data->Read(eyesStyle);
				std::uint32_t mouthStyle; data->Read(mouthStyle);
                std::uint8_t unknownB; data->Read(unknownB);

				if (unknownA != 0 || unknownB != 0) {
					Logger::log("CHAR-CREATION", "unknownA: " + std::to_string(unknownA));
					Logger::log("CHAR-CREATION", "unknownB: " + std::to_string(unknownB));
				}

				//PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), EDisconnectReason::CHARACTER_CORRUPTION);
				Database::CreateNewChar(GetDBConnection(), clientSession->accountID, s_customName, genname, headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle);
				
				PacketFactory::World::sendCharList(this, clientSession);
				
				break;
			}
			case EWorldPacketID::CLIENT_LOGIN_REQUEST: {

				DataTypes::LWOOBJID objectID;
				data->Read(objectID);
				clientSession->actorID = objectID;

				//PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), Enums::EDisconnectReason::PLAY_SCHEDULE_TIME_DONE);
				//PacketFactory::World::CreateCharacter(rakServer, clientSession);
				
				// PacketFactory::World::LoadStaticZone(rakServer, clientSession, luZone->zoneID, 0, 0, luZone->revisionChecksum, luZone->spawnPos.pos, 0);

				masterServerBridge->ClientCharAuth(clientSession, m_port, objectID);

				break;
			}
			case EWorldPacketID::CLIENT_GAME_MSG: {
				BitSize_t readOffset = data->GetReadOffset();

				GameMessages::Deserialize(this, clientSession, data);

				data->SetReadOffset(readOffset);
				DataTypes::LWOOBJID objectID;
				data->Read(objectID);
				std::uint16_t messageID;
				data->Read(messageID);

				Logger::log("WRLD", "Received Game Message ID #" + std::to_string(messageID));
				
				break;
			}
			case EWorldPacketID::CLIENT_LEVEL_LOAD_COMPLETE: {

				std::uint16_t zoneID;
				std::uint16_t mapInstance;
				std::uint32_t mapClone;

				data->Read(zoneID);
				data->Read(mapInstance);
				data->Read(mapClone);

				Logger::log("WRLD", "Client load complete ZoneID: " + std::to_string(zoneID) + " MapInstance: " + std::to_string(mapInstance) + " MapClone: " + std::to_string(mapClone));


				Logger::log("WRLD", "Construct player");
				replicaManager->AddParticipant(clientSession->systemAddress);

				auto * playerObject = new Entity::GameObject(this, 1);
				playerObject->SetObjectID(clientSession->actorID);
				auto * charComp = playerObject->GetComponent<CharacterComponent>();
				if (charComp != nullptr) {
					charComp->clientAddress = clientSession->systemAddress;
					DatabaseModels::Str_DB_CharInfo info = Database::GetChar(GetDBConnection(), clientSession->actorID.getPureID());
					playerObject->SetPosition(luZone->spawnPos.pos);
					playerObject->SetRotation(luZone->spawnPos.rot);


					auto charStyle = Database::GetCharStyle(GetDBConnection(), info.styleID);

					// shirt and pants got deleted, make new!
					if (info.shirtObjectID == 0ULL && info.pantsObjectID == 0ULL) {
						bool err = false;
						int shirtObjectLOT = 0;
						{
							FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
							if (!rth.isRowHeaderValid()) throw new std::runtime_error("Invalid Row Header");
							for (int i = 0; i < rth.getRowCount(); ++i) {
								if (!rth.isValid(i)) continue;
								try {
									if (
										*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 15 &&
										*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == charStyle.chestColor &&
										*reinterpret_cast<uint32_t*>(rth[i]["decal"].getMemoryLocation()) == charStyle.chest &&
										*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
										) {

										int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
										shirtObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
										break;
									}
								}
								catch (std::runtime_error e) {
									Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
								}
							}
							if (shirtObjectLOT == 0) {
								Logger::log("DB-CreateNewChar", "Unable to find componentID for shirt.");
								err = true;
							}
						}

						int pantsObjectLOT = 0; {
							FDB::RowTopHeader rth = Cache.getRows("ItemComponent");
							for (int i = 0; i < rth.getRowCount(); ++i) {
								if (!rth.isValid(i)) continue;
								try {
									if (
										*reinterpret_cast<uint32_t*>(rth[i]["itemType"].getMemoryLocation()) == 7 &&
										*reinterpret_cast<uint32_t*>(rth[i]["color1"].getMemoryLocation()) == charStyle.legs &&
										*reinterpret_cast<uint32_t*>(rth[i]["isBOE"].getMemoryLocation()) == 1
										) {
										int componentID = *reinterpret_cast<uint32_t*>(rth[i][0].getMemoryLocation());
										pantsObjectLOT = CacheComponentsRegistry::FindID(componentID, 11);
										break;
									}
								}
								catch (std::runtime_error e) {
									Logger::log("Cache:ItemComponent", e.what(), LogType::ERR);
								}
							}
							if (pantsObjectLOT == 0) {
								Logger::log("DB-CreateNewChar", "Unable to find componentID for pants.");
								err = true;
							}
						}

						if (!err) {
							auto shirtAndPants = Database::AddCharShirtAndPants(GetDBConnection(), playerObject->GetObjectID().getPureID(), shirtObjectLOT, pantsObjectLOT);
							info.shirtObjectID = std::get<0>(shirtAndPants).objectID;
							info.pantsObjectID = std::get<1>(shirtAndPants).objectID;
							Database::UpdateChar(GetDBConnection(), info);
						}
					}

					charComp->InitCharInfo(info);
					charComp->InitCharStyle(charStyle);
					charComp->CheckLevelProgression();
				
					auto* charDestComp = playerObject->GetComponent<DestructibleComponent>();
					if (charDestComp != nullptr) {
						charDestComp->SetImagination(info.imagination);
					}

					playerObject->SetName(std::u16string(info.name.begin(), info.name.end()));
					//playerObject->Finish();
				}
				auto invComp = playerObject->GetComponent<InventoryComponent>();
				
				// Bypass disabling of player construction
				// by missing components
				playerObject->isSerializable = true;

				objectsManager->RegisterObject(playerObject);
				playerObject->Finish();

				Logger::log("WRLD", "Create character packet");

				PacketFactory::World::CreateCharacter(rakServer, clientSession, playerObject);

				
				// Tell what world you are
				if (charComp != nullptr) {
					auto charInfo = charComp->GetCharInfo();
					charInfo.lastWorld = luZone->zoneID;
					charComp->InitCharInfo(charInfo);
					Database::UpdateChar(GetDBConnection(), charInfo);
				}

				Logger::log("WRLD", "Sending serialization");
				for (auto object_to_construct : objectsManager->GetObjects()) {
					if (object_to_construct->isSerializable && !object_to_construct->GetIsServerOnly() && object_to_construct->GetComponent<CharacterComponent>() == nullptr /*&& object_to_construct->GetComponent<PetComponent>() == nullptr*/) {
						Logger::log("WRLD", "Constructing LOT #" + std::to_string(object_to_construct->GetLOT()) +" ("+(std::string)CacheObjects::GetName(object_to_construct->GetLOT())+") with objectID "+std::to_string((unsigned long long)object_to_construct->GetObjectID()));
						objectsManager->Construct(object_to_construct, packet->getSystemAddress());
					}
				}

				

				//replicaManager->Construct(testStromling, false, UNASSIGNED_SYSTEM_ADDRESS, true);
				//replicaManager->Construct((Replica*)playerObject, false, UNASSIGNED_SYSTEM_ADDRESS, true);
				//objectsManager->RegisterObject(testStromling);
				
				for (auto object_to_construct : objectsManager->GetObjects()) {
					if (object_to_construct->GetObjectID() != playerObject->GetObjectID() && object_to_construct->isSerializable && (object_to_construct->GetComponent<CharacterComponent>() != nullptr /*|| object_to_construct->GetComponent<PetComponent>() != nullptr*/)) {
						Logger::log("WRLD", "Post-Load: Constructing LOT #" + std::to_string(object_to_construct->GetLOT()) + " (" + (std::string)CacheObjects::GetName(object_to_construct->GetLOT()) + ") with objectID " + std::to_string((unsigned long long)object_to_construct->GetObjectID()));
						objectsManager->Construct(object_to_construct, packet->getSystemAddress());
					}
				}

				if(invComp != nullptr)
					invComp->Awake();
				objectsManager->Construct(playerObject);

				Logger::log("WRLD", "Server done loading");
				PacketFactory::World::TestLoad(rakServer, clientSession);


				break;
			}
			case Enums::EWorldPacketID::CLIENT_POSITION_UPDATE: {
				DataTypes::LWOOBJID objectID = clientSession->actorID;
				Entity::GameObject * playerObject = objectsManager->GetObjectByID(objectID);
				if (playerObject != nullptr) {
					auto * controllablePhysicsComponent = playerObject->GetComponent<ControllablePhysicsComponent>();
					controllablePhysicsComponent->Deserialize(data);
				}
				else {
					// throw std::runtime_error("Invalid objectID; TODO: Kick Player -> Cheating");
				}
				break;
			}

			case Enums::EWorldPacketID::CLIENT_STRING_CHECK: {
				std::uint8_t chatMode, chatChannel;
				std::uint16_t string_length;
				data->Read(chatMode);
				data->Read(chatChannel);
				std::u16string private_name = StringUtils::readBufferedWStringFromBitStream(data, 84);
				data->Read(string_length);
				std::u16string message = StringUtils::readBufferedWStringFromBitStream(data, int(string_length * 2));
				
				PacketFactory::Chat::StringCheck(objectsManager->GetObjectByID(clientSession->actorID), chatMode, chatChannel, message);
				break;
			}

			case Enums::EWorldPacketID::CLIENT_GENERAL_CHAT_MESSAGE: {
				std::uint8_t unk1, unk2, unk3;
				data->Read(unk1);
				data->Read(unk2);
				data->Read(unk3);

				std::u16string chatMessage = StringUtils::readWStringFromBitStream<std::uint32_t>(data);

				Entity::GameObject* playerObject = objectsManager->GetObjectByID(clientSession->actorID);
				if (playerObject != nullptr) {
					PacketFactory::Chat::SendChatMessage(playerObject, 0x04, chatMessage);
				}

				break;
			}

			case (Enums::EWorldPacketID)91: {
				/* This is an unknown packet.*/
				std::uint8_t * pack = data->GetData();
				Logger::log("WRLD", "Received unknown packet containing: " + std::to_string(*reinterpret_cast<std::int32_t*>(pack)));
				break;
			}
			case (Enums::EWorldPacketID)30: {
				// This packet should kick if the client decides to spam it but it appears to be triggered in AGS atm so there is no point in adding the trigger
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
		
	} break;
	case ID_CONNECTION_LOST:
	case ID_DISCONNECTION_NOTIFICATION: {
		Logger::log("WRLD", "User Disconnected from WORLD...");
		ClientSession * session = sessionManager.GetSession(packet->getSystemAddress());
		if (session != nullptr) {
			// Remove the session locally
			sessionManager.RemoveSession(session);
			// Tell the master server about it
			masterServerBridge->ClientDisconnect(packet->getSystemAddress(), this);
			// Remove the player object
			Entity::GameObject* playerObject = objectsManager->GetObjectByID(session->actorID);
			if (playerObject != nullptr) {
				playerObject->InstantiateRemoval();
			}
		}
	} break;
	default: {
		Logger::log("WRLD", "Recieved unknown packet #" + (byte)packetHeader.packetID);
	}
	}

	// Deallocate the packet to conserve memory
	delete data;
	rakServer->DeallocatePacket(packet->getPacket());
}

void WorldServer::FinishClientTransfer(ClientSession clSession) {

	ClientSession * clSessionLocal = this->sessionManager.GetSession(clSession.accountID);

	*clSessionLocal = clSession;

	//PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), Enums::EDisconnectReason::PLAY_SCHEDULE_TIME_DONE);
	//PacketFactory::World::CreateCharacter(rakServer, clientSession);

	PacketFactory::World::LoadStaticZone(rakServer, clSessionLocal, luZone->zoneID, 0, 0, luZone->revisionChecksum, luZone->spawnPos.pos, 0);
}

void WorldServer::DisconnectPlayer(SystemAddress client, EDisconnectReason reason) {

	PacketFactory::General::doDisconnect(rakServer, client, reason);
}

std::uint16_t WorldServer::GetZoneID() {
	if(luZone != nullptr)
		return luZone->zoneID;
	return 0;
}

WorldServer::~WorldServer() {
	if (replicaManager) delete replicaManager;
	if (networkIdManager) delete networkIdManager;
	if (luZone) delete luZone;
	if (collisionConfiguration) delete collisionConfiguration;
	if (collisionDispatcher) delete collisionDispatcher;
	if (overlappingPairCache) delete overlappingPairCache;
	if (constraintSolver) delete constraintSolver;
	if (dynamicsWorld) delete dynamicsWorld;
	Database::Disconnect(GetDBConnection());
}
