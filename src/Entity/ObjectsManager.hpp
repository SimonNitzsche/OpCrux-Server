#ifndef __ENTITY__OBJECTS_MANAGER_HPP__
#define __ENTITY__OBJECTS_MANAGER_HPP__

#include <RakNet/ReplicaManager.h>
#include "Server/WorldServer.hpp"
#include "Entity/GameObject.hpp"

class WorldServer;
class SpawnerComponent;

class ObjectsManager {
private:
	WorldServer * Instance = nullptr;
	ReplicaManager * RM = nullptr;
	std::unordered_map<std::uint64_t, Entity::GameObject *> object_list;
	std::list<std::uint64_t> object_garbage;
	std::uint64_t spawnedObjectIDCounter = 0;
public:
	ObjectsManager();
	ObjectsManager(WorldServer * Instance);
	void Destroy();
	void RegisterObject(Entity::GameObject * object);
	void UnRegisterObject(DataTypes::LWOOBJID objID);
	void UnRegisterObject(Entity::GameObject * object);
	Entity::GameObject * GetObjectByID(DataTypes::LWOOBJID objID);
	std::vector<Entity::GameObject*> GetObjects();
    std::vector<Entity::GameObject*> GetControllableObjects();
    std::vector<Entity::GameObject*> GetObjectsInGroup(std::u16string groupName, DataTypes::LWOOBJID ignoreSelf = std::uint64_t(0), bool ignoreSpawners = false);
	void Construct(DataTypes::LWOOBJID objID, SystemAddress addr = UNASSIGNED_SYSTEM_ADDRESS);
	void Construct(Entity::GameObject * object, SystemAddress addr = UNASSIGNED_SYSTEM_ADDRESS);
	void Serialize();
	void Serialize(DataTypes::LWOOBJID objID);
	void Serialize(Entity::GameObject * object);
	void Destruct(DataTypes::LWOOBJID objID);
	void Destruct(Entity::GameObject * object);
	std::vector<Entity::GameObject*> GetObjectsInRange(DataTypes::Vector3 originPosition, std::float_t range, std::int32_t max = -1);
	void OnUpdate();
	void OnPhysicsUpdate();
	SpawnerComponent* GetSpawnerByName(std::u16string spawnerName);
    DataTypes::LWOOBJID GenerateSpawnedID();
	~ObjectsManager() {}
};

#endif