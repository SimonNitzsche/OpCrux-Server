#ifndef __REPLICA__COMPONENTS__RACING_CONTROL_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__RACING_CONTROL_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"

#include "Entity/Components/StatsComponent.hpp"
#include "Entity/Components/PhantomPhysicsComponent.hpp"
#include "Entity/Components/ScriptedActivityComponent.hpp"
#include "Entity/Components/ModuleAssemblyComponent.hpp"
#include "Entity/GameObject.hpp"
#include "GameCache/RebuildComponent.hpp"

#include "Enums/ERacingClientNotificationType.hpp"

#include "Entity/GameMessages/ActivityStart.hpp"
#include "Entity/GameMessages/NotifyRacingClient.hpp"
#include "Entity/GameMessages/RacingPlayerLoaded.hpp"
#include "Entity/Components/PossessableComponent.hpp"
#include "Entity/GameMessages/VehicleUnlockInput.hpp"


#include "FileTypes/LUZFile/LUZone.hpp"

using namespace DataTypes;

class RacingControlComponent : public ScriptedActivityComponent {
	friend class ScriptedActivityComponent;
private:

	std::uint16_t minNumOfPlayers = 2;

	struct RacingPlayerInfo {
		DataTypes::LWOOBJID playerObjectID;
		DataTypes::LWOOBJID carObjectID;
		std::uint32_t racingState;
		bool playerLoaded;
	};

	bool _playerInfoDirty = false;
	bool _playerInfo2Dirty = false;
	std::list<RacingPlayerInfo> playerInfo;

	std::uint16_t numberOfLaps = 3; // remainingLaps
	std::u16string pathName = u"MainPath";

public:

	RacingControlComponent(std::int32_t componentID) : ScriptedActivityComponent(componentID) {}

	static constexpr int GetTypeID() { return 71; }

	void OnEnable() {
		
	}

	void Awake() {
		
	}

	void msgPlayerAddedToWorldLocal(DataTypes::LWOOBJID playerID) {
		Entity::GameObject * playerObject = owner->GetZoneInstance()->objectsManager->GetObjectByID(playerID);

		if (playerObject == nullptr) return;

		// Make Car
		Entity::GameObject* myCar = new Entity::GameObject(this->owner->GetZoneInstance(), 8092);

		if (!myCar->isSerializable) {
			// Spawn Error Object
			delete myCar;
			myCar = new Entity::GameObject(this->owner->GetZoneInstance(), 1845);

		}

		// Set ObjectID
		myCar->SetObjectID(owner->GetZoneInstance()->objectsManager->GenerateSpawnedID());

		// Set Position/Rotation
		//spawnedObject->SetPosition(DataTypes::Vector3(-1475.7, 794.0, -351.6));
		//myCar->SetPosition(DataTypes::Vector3(-1.85433960, 203.026459, -27.7652206));
		//myCar->SetRotation(DataTypes::Quaternion(0, 0.8638404011726379, 0, 0.5037656426429749));

		auto path = reinterpret_cast<FileTypes::LUZ::LUZonePathRace*>(owner->GetZoneInstance()->luZone->paths.find(u"MainPath")->second);
		myCar->SetPosition(path->waypoints.at(0)->position);
		//myCar->SetRotation(DataTypes::Quaternion(0, 0.8638404011726379, 0, 0.5037656426429749));


		auto modAComp = myCar->GetComponent<ModuleAssemblyComponent>();

		modAComp->SetAssembly(u"1:8129;1:8130;1:13513;1:13512;1:13515;1:13516;1:13514;");


		// Set Parent
		myCar->SetParent(this->owner->GetZoneInstance()->zoneControlObject);

		// Finish & Send car
		myCar->Finish();

		// Register
		this->owner->GetZoneInstance()->objectsManager->RegisterObject(myCar);

		// Construct
		if (myCar->isSerializable)
			this->owner->GetZoneInstance()->objectsManager->Construct(myCar);


		// Add player to activity
		this->AddPlayerToActivity(playerID);

		// Add player to race
		RacingPlayerInfo pi;
		pi.playerObjectID = playerID;
		pi.carObjectID = myCar->GetObjectID();
		pi.racingState = 0;
		pi.playerLoaded = false;
		playerInfo.push_back(pi);
		_playerInfoDirty = true;
		this->owner->SetDirty();

		// Tell player car is ready and added to race
		this->owner->GetZoneInstance()->objectsManager->Serialize(this->owner);

		/*
			TODO: Player Ready & Teleport & ZoneObject: PlayerReady
			TODO: GetLastCustomBuild
		*/
		

		playerObject->Possess(myCar);

		{ GM::NotifyVehicleOfRacingObject msg; msg.racingObjectID = this->owner->GetObjectID(); GameMessages::Broadcast(this->owner->GetZoneInstance(), myCar, msg); }

		{ GM::RacingPlayerLoaded msg; msg.playerID = playerID; msg.vehicleID = myCar->GetObjectID(); GameMessages::Broadcast(owner->GetZoneInstance(), owner, msg); }

		{GM::VehicleUnlockInput msg; msg.bLockWheels = false; GameMessages::Broadcast(owner->GetZoneInstance(), myCar, msg); }
	}

	void OnAcknowledgePossession(Entity::GameObject* player, Entity::GameObject* car) {

		//DataTypes::Vector3 pos = playerObject->GetPosition();
		//pos.y += 10;
		//playerObject->SetPosition(pos);
		////playerObject->SetPosition(DataTypes::Vector3(-1475.7, 794.0, -351.6));
		//playerObject->SetDirty();






		

		//Test();
		this->owner->GetZoneInstance()->objectsManager->Serialize(this->owner);

		//{ GM::NotifyRacingClient msg; msg.eventType = Enums::ERacingClientNotificationType::ACTIVITY_START; GameMessages::Broadcast(owner->GetZoneInstance(), owner, msg); }
		//{ GM::ActivityStart msg; GameMessages::Broadcast(owner->GetZoneInstance(), owner, msg); }
	}

	void Update() {
		
	}

	void Start() {
		
	}

	void PopulateFromLDF(LDFCollection* collection) {
		
	}

	void Serialize(RakNet::BitStream* factory, ReplicaTypes::PacketTypes packetType) {

		ScriptedActivityComponent::Serialize(factory, packetType);

		factory->Write(true);
		factory->Write<std::uint16_t>(minNumOfPlayers);

		factory->Write(_playerInfoDirty && playerInfo.size() != 0);
		if (_playerInfoDirty && playerInfo.size() != 0) {
			auto it = playerInfo.begin();
			while (true) {
				factory->Write(it == playerInfo.end());
				if (it == playerInfo.end()) {
					break;
				}

				factory->Write(it->playerObjectID);
				factory->Write(it->carObjectID);
				factory->Write(it->racingState);
				factory->Write(it->playerLoaded);
				++it;
			}
		}

		factory->Write(_playerInfo2Dirty && playerInfo.size() != 0);
		if (_playerInfo2Dirty && playerInfo.size() != 0) {
			auto it = playerInfo.begin();
			while (true) {
				factory->Write(it == playerInfo.end());
				if (it == playerInfo.end()) {
					break;
				}

				factory->Write(it->playerObjectID);
				factory->Write(0);
				++it;
			}
		}

		factory->Write(packetType == ReplicaTypes::PacketTypes::CONSTRUCTION);
		if (packetType == ReplicaTypes::PacketTypes::CONSTRUCTION) {
			factory->Write(numberOfLaps);
			StringUtils::writeWStringToBitStream<std::uint16_t>(factory, pathName);
		}
		factory->Write(false);

	}

};

#endif