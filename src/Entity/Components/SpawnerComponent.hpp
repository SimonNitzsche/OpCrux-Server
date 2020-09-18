#ifndef __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__
#define __REPLICA__COMPONENTS__SPAWNER_COMPONENT_HPP__

#include <deque>

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
#include "Entity/Components/PhantomPhysicsComponent.hpp"

using namespace DataTypes;

class SpawnerComponent : public IEntityComponent {
private:
	LDFCollection ldfCache;
	std::uint32_t spawnCount=0;
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
	std::u16string respawnVolumeName;
	DataTypes::Vector3 respawnPosition;
	DataTypes::Quaternion respawnRotation;
	bool spawnerActiveOnLoad=false;
	std::int32_t spawnTemplate;
	bool networkSpawner=false;
	std::deque<std::uint64_t> respawnTasks = {};
public:
	DataTypes::Vector3 originPos;
	DataTypes::Quaternion originRot;
public:

	SpawnerComponent(std::int32_t componentID) : IEntityComponent(componentID) {}

	static constexpr int GetTypeID() { return 10; }

	void OnEnable() {
		owner->isSerializable = false;
	}

	void Awake() {
		if (spawnerActiveOnLoad) {
			Spawn(false);
		}
	}

	void Update() {
		std::uint64_t currentTime = ::time(0);
		std::uint32_t taskNumToRespawn = 0;
		// Look how many tasks to respawn
		for (auto it = this->respawnTasks.begin(); it != this->respawnTasks.end(); ++it) {
			if (*it <= currentTime) ++taskNumToRespawn;
			else break;
		}

		if (taskNumToRespawn == 0) return;

		// Respawn them
		for (int i = 0; i < taskNumToRespawn; ++i) {
			Spawn();
		}
		// Remove tasks
		respawnTasks.erase(respawnTasks.begin(), respawnTasks.begin() + taskNumToRespawn);
	}

	// When an object is deleted, this function is getting called before
	void NotifyOfObjectRemoval(Entity::GameObject* spawnedObject) {

		// Tell the spawner to automatically respawn object
		AddRespawnTask();
	}

	void AddRespawnTask() {
		std::int64_t x = std::int64_t(::time(0)) + std::int64_t(respawnTime);
		this->respawnTasks.push_back(x);
		//this->respawnTasks.push_back(::time(0));
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
		----------------------------------
		is_network_spawner:7
		spawner_max_per_node:1
		spawner_node_id:1
		weight:1
	*/

		LDF_GET_VAL_FROM_COLLECTION(maxToSpawn, collection, u"max_to_spawn", -1);
		LDF_GET_VAL_FROM_COLLECTION(noAutoSpawn, collection, u"no_auto_spawn", false);
		LDF_GET_VAL_FROM_COLLECTION(noTimedSpawn, collection, u"no_timed_spawn", true);
		LDF_GET_VAL_FROM_COLLECTION(respawnTime, collection, u"respawn", CacheWorldConfig::GetDefaultRespawnTime());
		if (respawnTime == 0.0f) respawnTime = CacheWorldConfig::GetDefaultRespawnTime();
		LDF_GET_VAL_FROM_COLLECTION(respawnVolume, collection, u"respawnVol", false);
		LDF_GET_VAL_FROM_COLLECTION(respawnVolumeName, collection, u"respawnVolName", u"");

		std::u16string wStrPos, wStrRot;
		LDF_GET_VAL_FROM_COLLECTION(wStrPos, collection, u"rspPos", u"NULL");
		std::vector<std::u16string> vecPos = StringUtils::splitWString(wStrPos, 0x001f);
		if(wStrPos != u"NULL")
			respawnPosition = DataTypes::Vector3(std::stof(StringUtils::to_string(vecPos.at(0))), std::stof(StringUtils::to_string(vecPos.at(1))), std::stof(StringUtils::to_string(vecPos.at(2))));

		LDF_GET_VAL_FROM_COLLECTION(wStrRot, collection, u"rspRot", u"NULL");
		std::vector<std::u16string> vecRot = StringUtils::splitWString(wStrRot, 0x001f);
		if (wStrRot != u"NULL")
			respawnRotation = DataTypes::Quaternion(std::stof(StringUtils::to_string(vecRot.at(0))), std::stof(StringUtils::to_string(vecRot.at(1))), std::stof(StringUtils::to_string(vecRot.at(2))), std::stof(StringUtils::to_string(vecRot.at(3))));

		LDF_GET_VAL_FROM_COLLECTION(spawnerActiveOnLoad, collection, u"spawner_active_on_load", true);
		LDF_GET_VAL_FROM_COLLECTION(spawnTemplate, collection, u"spawntemplate", -1);
	}

	void SetLUZPath() {

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

		// Set Scale
		spawnedObject->SetScale(this->owner->GetScale());

		// Populate LDF
		spawnedObject->PopulateFromLDF(&ldfCache);

		// Set Parent
		//spawnedObject->SetParent(this->owner);

		// Set Spawner
		spawnedObject->SetSpawner(this->owner, -1);

		// Set Position/Rotation
		spawnedObject->SetPosition(originPos);
		spawnedObject->SetRotation(originRot);

		spawnedObject->Finish();

		// Register
		Instance->objectsManager->RegisterObject(spawnedObject);

		// Construct
		if (construct && spawnedObject->isSerializable)
			Instance->objectsManager->Construct(spawnedObject);


		

		return true;
	}

};

#endif