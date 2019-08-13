#ifndef __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__

#include "Entity/Components/Interface/IEntityComponent.hpp"
#include "Entity/GameObject.hpp"

#include "GameCache/ComponentsRegistry.hpp"
#include "GameCache/MovementAIComponent.hpp"
#include "Utils/ServerInfo.hpp"

#include "DataTypes/Vector3.hpp"
#include "DataTypes/Quaternion.hpp"

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
	bool spawnerActiveOnLoad;
	std::int32_t spawnTemplate;

public:

	SpawnerComponent() : IEntityComponent() {}

	void OnEnable() {

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
	}

	// Returns true on success and false on fail
	bool Spawn(bool construct=true) {
		WorldServer * Instance = this->owner->GetZoneInstance();
		
		// Create
		Entity::GameObject * spawnedObject = new Entity::GameObject(Instance, spawnTemplate);
		
		// Set ObjectID
		spawnedObject->SetObjectID(LWOOBJID()); // TODO: Generate local objectID
		
		// Populate LDF
		spawnedObject->PopulateFromLDF(&ldfCache);

		// Set Parent
		spawnedObject->SetParent(this->owner);

		// Register
		Instance->objectsManager->RegisterObject(spawnedObject);

		// Construct
		if (construct)
			Instance->objectsManager->Construct(spawnedObject);

		return true;
	}

};

#endif