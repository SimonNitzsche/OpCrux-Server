#ifndef __ENTITY__OBJECTS_MANAGER_HPP__
#define __ENTITY__OBJECTS_MANAGER_HPP__

#include <RakNet/ReplicaManager.h>
#include "Server/WorldServer.hpp"
#include "Entity/GameObject.hpp"

class WorldServer;

class ObjectsManager {
private:
	WorldServer * Instance = nullptr;
	ReplicaManager * RM = nullptr;
	std::unordered_map<std::uint64_t, Entity::GameObject *> object_list;
public:
	ObjectsManager();
	ObjectsManager(WorldServer * Instance);
	void Destroy();
	void RegisterObject(Entity::GameObject * object);
	void UnRegisterObject(DataTypes::LWOOBJID objID);
	void UnRegisterObject(Entity::GameObject * object);
	Entity::GameObject * GetObjectByID(DataTypes::LWOOBJID objID);
	std::vector<Entity::GameObject*> GetObjects();
	void Construct(DataTypes::LWOOBJID objID);
	void Construct(Entity::GameObject * object);
	void Serialize();
	void Serialize(DataTypes::LWOOBJID objID);
	void Serialize(Entity::GameObject * object);
	void Destruct(DataTypes::LWOOBJID objID);
	void Destruct(Entity::GameObject * object);
	void OnUpdate();
	~ObjectsManager() {}
};

#endif