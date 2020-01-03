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

#include "Exceptions/ExNetException.hpp"

#include "Utils/Logger.hpp"
#include "Utils/ServerInfo.hpp"
#include "Utils/StringUtils.hpp"

#include "Structs/Networking/General/StructPacketHeader.hpp"

#include "PacketFactory/General/GeneralPackets.hpp"
#include "PacketFactory/World/WorldPackets.hpp"
#include <fstream>
#include "Database/Database.hpp"

#include "DataTypes/LWOOBJID.hpp"

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/Components/CharacterComponent.hpp"
#include "Entity/Components/SpawnerComponent.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"

#include "Entity/GameObject.hpp"

#include "GameCache/ZoneTable.hpp"
#include "GameCache/Objects.hpp"

#include "Utils/LDFUtils.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"

#include "bullet3-2.89/src/LinearMath/btIDebugDraw.h"


#include "Entity/GameMessages.hpp"
using namespace Exceptions;

extern BridgeMasterServer* masterServerBridge;

WorldServer::WorldServer(int zone, int instanceID, int port) {
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

	// Initializes SocketDescriptor
	SocketDescriptor socketDescriptor((unsigned short)2001, 0);
	Logger::log("WRLD", "Starting World...");

	rakServer->SetMaximumIncomingConnections((unsigned short)2);

	replicaManager = new ReplicaManager();
	rakServer->AttachPlugin(replicaManager);
	replicaManager->SetDefaultScope(true);
	networkIdManager = new NetworkIDManager();
	rakServer->SetNetworkIDManager(networkIdManager);

	replicaManager->SetAutoParticipateNewConnections(true);
	replicaManager->SetAutoSerializeInScope(true);

	networkIdManager->SetIsNetworkIDAuthority(true);

	this->objectsManager = new ObjectsManager(this);

	// Check startup
	if (!rakServer->Startup(2, 30, &socketDescriptor, 1)) {
		std::cin.get();
		return;
	}

	/*
		LOAD WORLD STUFF. Zone 0 is char selection only, so we don't need to load world.
	*/

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
		Logger::log("WRLD", "Sucessfully loaded zone.");

		if (luZone->zoneID != zone) {
			Logger::log("WRLD", "Invalid zoneID within LUZ file, correcting...", LogType::UNEXPECTED);
			luZone->zoneID = zone;
		}

		std::int32_t zoneControlLOT = CacheZoneTable::GetZoneControlTemplate(zone);
		if (zoneControlLOT == 0) zoneControlLOT = 2365;
		this->zoneControlObject = new Entity::GameObject(this, zoneControlLOT);
		this->zoneControlObject->SetObjectID(0x3FFFFFFFFFFE);
		objectsManager->RegisterObject(this->zoneControlObject);
		this->zoneControlObject->Finish();

		// TODO: Move this to somewhere else
		for (auto scene : luZone->scenes) {
			for (auto objT : scene.scene.objectsChunk.objects) {
				Entity::GameObject* go = new Entity::GameObject(this, *objT.LOT);
				go->SetObjectID(DataTypes::LWOOBJID(/*(1ULL << 45)*/ 70368744177664ULL | *objT.objectID));
				LDFCollection ldfCollection = LDFUtils::ParseCollectionFromWString(objT.config.ToString());

				// If Spawner
				SpawnerComponent* spawnerComp = static_cast<SpawnerComponent*>(go->GetComponentByType(10));
				if (spawnerComp != nullptr) {
					spawnerComp->originPos = objT.spawnPos->pos;
					spawnerComp->originRot = objT.spawnPos->rot;
				}
				go->isSerializable = false;

				go->SetScale(*objT.scale);

				go->PopulateFromLDF(&ldfCollection);

				objectsManager->RegisterObject(go);
			}
		}

		// Path's Network Spawner
		for (auto pathBase : luZone->paths) {
			if (pathBase.second->pathType == FileTypes::LUZ::LUZonePathType::Spawner) {
				auto spawnerPath = reinterpret_cast<FileTypes::LUZ::LUZonePathSpawner*>(pathBase.second);
				WorldServer* Instance = this;

				if (spawnerPath->spawnedLOT == 0) continue;

				for (int i = 0; i < spawnerPath->numberToMaintain; ++i) {
					// Create
					Entity::GameObject* spawnedObject = new Entity::GameObject(Instance, spawnerPath->spawnedLOT);


					if (!spawnedObject->isSerializable) {
						// Spawn Error Object
						delete[] spawnedObject;
						spawnedObject = new Entity::GameObject(Instance, 1845);

					}

					// Set ObjectID
					spawnedObject->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + Instance->spawnedObjectIDCounter++));
					//spawnedObject->SetObjectID(DataTypes::LWOOBJID(288334496658198694ULL + Instance->spawnedObjectIDCounter++));


					// Populate LDF
					spawnedObject->PopulateFromLDF(&spawnerPath->waypoints.at(i)->config);

					// Set Position/Rotation
					ControllablePhysicsComponent* controllablePhysicsComponent = spawnedObject->GetComponent<ControllablePhysicsComponent>();
					if (controllablePhysicsComponent != nullptr) {
						controllablePhysicsComponent->SetPosition(spawnerPath->waypoints.at(i)->position);
						controllablePhysicsComponent->SetRotation(spawnerPath->waypoints.at(i)->rotation);
					}
					SimplePhysicsComponent* simplePhysicsComponent = spawnedObject->GetComponent<SimplePhysicsComponent>();
					if (simplePhysicsComponent != nullptr) {
						simplePhysicsComponent->SetPosition(spawnerPath->waypoints.at(i)->position);
						simplePhysicsComponent->SetRotation(spawnerPath->waypoints.at(i)->rotation);
					}

					// Register
					Instance->objectsManager->RegisterObject(spawnedObject);
				}
			}
		}


		// Finish objects
		for (auto go : this->objectsManager->GetObjects()) {
			go->Finish();
		}
	}

	Packet* packet;
	initDone = true;

	Logger::log("WRLD", "Finished loading and initializing of the instance.", LogType::PASSED);

	std::thread glT([=]() { GameLoopThread(); });
	glT.detach();

	std::thread gpT([=]() { GamePhysicsThread(); });
	gpT.detach();

	while (ServerInfo::bRunning) {
		RakSleep(30);
		while (packet = rakServer->Receive()) {
			try {
				handlePacket(rakServer, reinterpret_cast<LUPacket*>(packet));
			}
			catch (NetException::CorruptPacket e) {
				Logger::log("WRLD", "Received corrupt packet.", LogType::ERR);
				PacketFactory::General::doDisconnect(rakServer, packet->systemAddress, EDisconnectReason::UNKNOWN_SERVER_ERROR);
			}
			catch (std::runtime_error * e) {
				Logger::log("WRLD", "[CRASH] " + std::string(e->what()), LogType::ERR);
				throw new std::runtime_error(*e);
			}
		}
	}

	// QUIT
	Logger::log("WRLD", "Recieved QUIT, shutting down...");

	rakServer->Shutdown(0);
	RakNetworkFactory::DestroyRakPeerInterface(rakServer);
}

void WorldServer::GameLoopThread() {
	while (ServerInfo::bRunning) {
		objectsManager->OnUpdate();
		timer.Update();
		RakSleep(300);
	}
}

void WorldServer::GamePhysicsThread() {
	while (ServerInfo::bRunning) {
		dynamicsWorld->stepSimulation(0.0166667f, 10);
		objectsManager->OnPhysicsUpdate();
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

			switch (static_cast<EWorldPacketID>(packetHeader.packetID)) {
			case EWorldPacketID::CLIENT_VALIDATION: {
				std::u16string wClientName = StringUtils::readBufferedWStringFromBitStream(data);
				std::u16string wClientKey = StringUtils::readBufferedWStringFromBitStream(data);
				std::string sClientFDBChecksum = StringUtils::readBufferedStringFromBitStream(data);
				ClientSession csFactory;
				csFactory.accountID = Database::GetAccountIDByClientName(std::string(wClientName.begin(), wClientName.end()));
				csFactory.sessionToken = wClientKey;
				csFactory.systemAddress = packet->getSystemAddress();

				sessionManager.AddSession(csFactory);

				masterServerBridge->ClientWorldAuth(packet->getSystemAddress(), csFactory.accountID);
				break;
			}
			case EWorldPacketID::CLIENT_CHARACTER_LIST_REQUEST: {
				PacketFactory::World::sendCharList(rakServer, clientSession);
				break;
			}
			case EWorldPacketID::CLIENT_CHARACTER_CREATE_REQUEST: {

				std::u16string customName = StringUtils::readBufferedWStringFromBitStream(data);
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
				Database::CreateNewChar(clientSession->accountID, s_customName, genname, headColor, head, chestColor, chest, legs, hairStyle, hairColor, leftHand, rightHand, eyebrowStyle, eyesStyle, mouthStyle);
				
				PacketFactory::World::sendCharList(rakServer, clientSession);
				
				break;
			}
			case EWorldPacketID::CLIENT_LOGIN_REQUEST: {

				DataTypes::LWOOBJID objectID;
				data->Read(objectID);
				clientSession->actorID = objectID;

				//PacketFactory::General::doDisconnect(rakServer, packet->getSystemAddress(), Enums::EDisconnectReason::PLAY_SCHEDULE_TIME_DONE);
				//PacketFactory::World::CreateCharacter(rakServer, clientSession);
				
				PacketFactory::World::LoadStaticZone(rakServer, clientSession, luZone->zoneID, 0, 0, luZone->revisionChecksum, luZone->spawnPos.pos, 0);
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
				
				// StartSkill, we use it currently for debugging purpose.
				// Following code logs the nearest object around the player when punching.
				if (messageID == 0x77) {
					Entity::GameObject * player = objectsManager->GetObjectByID(objectID);
					DataTypes::Vector3 playerPos = player->GetPosition();
					float nearestDistance = INFINITY;
					Entity::GameObject * nearestObject = nullptr;
					for (auto obj : objectsManager->GetObjects()) {
						if (obj != player) {
							DataTypes::Vector3 objPos;
							objPos = obj->GetPosition();

							float distance = Vector3::Distance(playerPos, objPos);
							if (distance < nearestDistance) {
								nearestDistance = distance;
								nearestObject = obj;
							}
						}
					}

					Logger::log("WRLD", "Nearest Object is " + (std::string)CacheObjects::GetName(nearestObject->GetLOT()) + " with LOT " + std::to_string(nearestObject->GetLOT()) + " as " + std::to_string((std::uint64_t)nearestObject->GetObjectID()));
				}

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

				
				/*Entity::GameObject * testStromling = new Entity::GameObject(this, 4712);
				testStromling->SetObjectID(288334496658198693ULL); // Random ID
				ControllablePhysicsComponent * scpComp = (ControllablePhysicsComponent*)testStromling->GetComponentByID(1);
				scpComp->SetPosition(luZone->spawnPos->pos);
				scpComp->SetRotation(Quaternion(0,0,0,0));*/

				Logger::log("WRLD", "Construct player");
				replicaManager->AddParticipant(clientSession->systemAddress);
				Entity::GameObject * playerObject = new Entity::GameObject(this, 1);
				playerObject->SetObjectID(clientSession->actorID);
				CharacterComponent * charComp = playerObject->GetComponent<CharacterComponent>();
				charComp->clientAddress = clientSession->systemAddress;
				Database::Str_DB_CharInfo info = Database::GetChar(clientSession->actorID.getPureID());
				playerObject->SetPosition(luZone->spawnPos.pos);
				playerObject->SetRotation(luZone->spawnPos.rot);

				charComp->InitCharInfo(info);
				charComp->InitCharStyle(Database::GetCharStyle(info.styleID));
				DestructibleComponent* charDestComp = playerObject->GetComponent<DestructibleComponent>();
				charDestComp->SetImagination(info.imagination);

				playerObject->SetName(std::u16string(info.name.begin(), info.name.end()));
				
				// Bypass disabling of player construction
				// by missing components
				playerObject->isSerializable = true;

				objectsManager->RegisterObject(playerObject);

				Logger::log("WRLD", "Create character packet");

				PacketFactory::World::CreateCharacter(rakServer, clientSession, playerObject);

				
				
				if (zoneID == 1203) {
					// Racing tests, remove in future!

					playerObject->SetPosition(DataTypes::Vector3(-1457.71f, 2794.0f, -351.61f));

				}


				Logger::log("WRLD", "Sending serialization");
				for (auto object_to_construct : objectsManager->GetObjects()) {
					if (object_to_construct->isSerializable && object_to_construct->GetComponent<CharacterComponent>() == nullptr /*&& object_to_construct->GetComponent<PetComponent>() == nullptr*/) {
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

				objectsManager->Construct(playerObject);

				Logger::log("WRLD", "Server done loading");
				PacketFactory::World::TestLoad(rakServer, clientSession);

				

				break;
			}
			case Enums::EWorldPacketID::CLIENT_POSITION_UPDATE: {
				DataTypes::LWOOBJID objectID = clientSession->actorID;
				Entity::GameObject * playerObject = objectsManager->GetObjectByID(objectID);
				if (playerObject != nullptr) {
					ControllablePhysicsComponent * controllablePhysicsComponent = playerObject->GetComponent<ControllablePhysicsComponent>();
					controllablePhysicsComponent->Deserialize(data);
				}
				else {
					throw new std::runtime_error("Invalid objectID; TODO: Kick Player -> Cheating");
				}
				break;
			}

			case (Enums::EWorldPacketID)91: {
				/* This is an unknown packet.*/
				unsigned char * pack = data->GetData();
				Logger::log("WRLD", "Received unknown packet containing: " + std::to_string(*reinterpret_cast<std::int32_t*>(pack)));
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
		
	} break;
	case ID_DISCONNECTION_NOTIFICATION: {
		Logger::log("WRLD", "User Disconnected from WORLD...");
		ClientSession * session = sessionManager.GetSession(packet->getSystemAddress());
		if (session != nullptr) {
			objectsManager->Destruct(session->actorID);
			sessionManager.RemoveSession(session);
			masterServerBridge->ClientDisconnect(packet->getSystemAddress());
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

WorldServer::~WorldServer() {
	if (replicaManager) delete[] replicaManager;
	if (networkIdManager) delete[] networkIdManager;
	if (luZone) delete[] luZone;
	if (collisionConfiguration) delete collisionConfiguration;
	if (collisionDispatcher) delete collisionDispatcher;
	if (overlappingPairCache) delete overlappingPairCache;
	if (constraintSolver) delete constraintSolver;
	if (dynamicsWorld) delete dynamicsWorld;
}
