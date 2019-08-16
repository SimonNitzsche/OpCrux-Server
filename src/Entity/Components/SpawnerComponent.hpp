#ifndef __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "GameCache/WorldConfig.hpp"
#include "Utils/ServerInfo.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

#include "Utils/LDFUtils.hpp"

#include "Entity/Components/SimplePhysicsComponent.hpp"
#include "Entity/Components/ControllablePhysicsComponent.hpp"

using namespace DataTypes;

class SpawnerComponent : public IEntityComponent {
private:
	LDFCollection ldfCache;

private:
	/*
		Used LDF Values:
		----------------
		max_to_spawn:1 -> maxToSpawn
		no_auto_spawn:7 -> noAutoSpawn
		no_timed_spawn:7 -> noTimedSpawn
		respawn:3 -> respawnTime
		respawnVol:7 -> respawnVolume;
		respawnVolName:0 -> respawnVolumeName
		rspPos:0 -> respawnPosition
		rspRot:0 -> respawnRotation
		spawner_active_on_load:7 -> spawnerActiveOnLoad
		spawntemplate:1 -> spawnTemplate
	*/
	std::int32_t maxToSpawn;
	bool noAutoSpawn;
	bool noTimedSpawn;
	float respawnTime;
	bool respawnVolume;
	std::wstring respawnVolumeName;
	DataTypes::Vector3 respawnPosition;
	DataTypes::Quaternion respawnRotation;
	bool spawnerActiveOnLoad=false;
	std::int32_t spawnTemplate;
public:
	DataTypes::Vector3 originPos;
	DataTypes::Quaternion originRot;
public:

	SpawnerComponent() : IEntityComponent() {}

	void OnEnable() {
		owner->isSerializable = false;
	}

	void Awake() {
		if (spawnerActiveOnLoad) {
			Spawn(false);
		}
	}

	void Update() {

	}

	void PopulateFromLDF(LDFCollection * collection) {
		ldfCache = *collection;

		/*
		Used LDF Values:
		----------------
		max_to_spawn:1 -> maxToSpawn
		no_auto_spawn:7 -> noAutoSpawn
		no_timed_spawn:7 -> noTimedSpawn
		respawn:3 -> respawnTime
		respawnVol:7 -> respawnVolume;
		respawnVolName:0 -> respawnVolumeName
		rspPos:0 -> respawnPosition
		rspRot:0 -> respawnRotation
		spawner_active_on_load:7 -> spawnerActiveOnLoad
		spawntemplate:1 -> spawnTemplate
	*/

		LDF_GET_VAL_FROM_COLLECTION(maxToSpawn, collection, L"max_to_spawn", -1);
		LDF_GET_VAL_FROM_COLLECTION(noAutoSpawn, collection, L"no_auto_spawn", false);
		LDF_GET_VAL_FROM_COLLECTION(noTimedSpawn, collection, L"no_timed_spawn", true);
		LDF_GET_VAL_FROM_COLLECTION(respawnTime, collection, L"respawn", CacheWorldConfig::GetDefaultRespawnTime());
		LDF_GET_VAL_FROM_COLLECTION(respawnVolume, collection, L"respawnVol", false);
		LDF_GET_VAL_FROM_COLLECTION(respawnVolumeName, collection, L"respawnVolName", L"");

		std::wstring wStrPos, wStrRot;
		LDF_GET_VAL_FROM_COLLECTION(wStrPos, collection, L"rspPos", L"NULL");
		std::vector<std::wstring> vecPos = StringUtils::splitWString(wStrPos, 0x001f);
		if(wStrPos != L"NULL")
			respawnPosition = DataTypes::Vector3(std::stof(vecPos.at(0)), std::stof(vecPos.at(1)), std::stof(vecPos.at(2)));

		LDF_GET_VAL_FROM_COLLECTION(wStrRot, collection, L"rspRot", L"NULL");
		std::vector<std::wstring> vecRot = StringUtils::splitWString(wStrRot, 0x001f);
		if(wStrRot != L"NULL")
			respawnRotation = DataTypes::Quaternion(std::stof(vecRot.at(0)), std::stof(vecRot.at(1)), std::stof(vecRot.at(2)), std::stof(vecRot.at(3)));

		LDF_GET_VAL_FROM_COLLECTION(spawnerActiveOnLoad, collection, L"spawner_active_on_load", true);
		LDF_GET_VAL_FROM_COLLECTION(spawnTemplate, collection, L"spawntemplate", -1);

		Spawn(false);
	}

	// Returns true on success and false on fail
	bool Spawn(bool construct=true) {
		WorldServer * Instance = this->owner->GetZoneInstance();
		
		// Create
		Entity::GameObject * spawnedObject = new Entity::GameObject(Instance, spawnTemplate);
		
		
		if (!spawnedObject->isSerializable) {
			// Spawn Error Object
			delete[] spawnedObject;
			spawnedObject = new Entity::GameObject(Instance, 1845);

		}

		// Set ObjectID
		spawnedObject->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + Instance->spawnedObjectIDCounter++));
		//spawnedObject->SetObjectID(DataTypes::LWOOBJID(288334496658198694ULL + Instance->spawnedObjectIDCounter++));


		// Populate LDF
		spawnedObject->PopulateFromLDF(&ldfCache);

		// Set Parent
		//spawnedObject->SetParent(this->owner);

		// Set Position/Rotation
		ControllablePhysicsComponent * controllablePhysicsComponent = static_cast<ControllablePhysicsComponent*>(spawnedObject->GetComponentByID(1));
		if (controllablePhysicsComponent != nullptr) {
			controllablePhysicsComponent->SetPosition(originPos);
			controllablePhysicsComponent->SetRotation(originRot);
		}
		SimplePhysicsComponent * simplePhysicsComponent = static_cast<SimplePhysicsComponent*>(spawnedObject->GetComponentByID(3));
		if (simplePhysicsComponent != nullptr) {
			simplePhysicsComponent->SetPosition(originPos);
			simplePhysicsComponent->SetRotation(originRot);
		}


		// Register
		Instance->objectsManager->RegisterObject(spawnedObject);

		// Construct
		if (construct && spawnedObject->isSerializable)
			Instance->objectsManager->Construct(spawnedObject);


		

		return true;
	}

};

#endif