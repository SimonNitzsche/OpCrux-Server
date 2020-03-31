#include "Entity/ObjectsManager.hpp"

#include "bullet3-2.89/src/btBulletDynamicsCommon.h"

ObjectsManager::ObjectsManager()
{
}

ObjectsManager::ObjectsManager(WorldServer * Instance) {
	this->Instance = Instance;
	this->RM = this->Instance->replicaManager;
}

void ObjectsManager::Destroy() {
	for (auto oPair : this->object_list) {
		this->Destruct(oPair.second);
	}
	object_list.clear();
}

void ObjectsManager::RegisterObject(Entity::GameObject * object) {
	if (object == nullptr) return;
	object_list.insert(std::make_pair(object->GetObjectID(), object));
}

void ObjectsManager::UnRegisterObject(DataTypes::LWOOBJID objID) {
	object_list.erase(objID);
}

void ObjectsManager::UnRegisterObject(Entity::GameObject * object) {
	UnRegisterObject(object->GetObjectID());
}

Entity::GameObject * ObjectsManager::GetObjectByID(DataTypes::LWOOBJID objID) {
	auto it = object_list.find(objID);
	if (it != object_list.end())
		return it->second;
	return nullptr;
}

std::vector<Entity::GameObject*> ObjectsManager::GetObjects() {
	std::vector<Entity::GameObject*> out;
	for (auto oPair : object_list)
		out.push_back(oPair.second);
	return out;
}

std::vector<Entity::GameObject*> ObjectsManager::GetObjectsInGroup(std::u16string groupName) {
	std::vector<Entity::GameObject*> out;
	for (auto oPair : object_list) {
		if (oPair.second->IsWithinGroup(groupName)) {
			out.push_back(oPair.second);
		}
	}
	return out;
}

void ObjectsManager::Construct(DataTypes::LWOOBJID objID, SystemAddress addr) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject != nullptr)
		Construct(gameObject);
	else {
		Logger::log("WRLD", "Error: Cannot construct an object that does not exist! objID: " + std::to_string(objID), LogType::ERR);
	}
}

void ObjectsManager::Construct(Entity::GameObject * object, SystemAddress addr) {
	RM->Construct(object, false, addr, addr == UNASSIGNED_SYSTEM_ADDRESS);
}

void ObjectsManager::Serialize() {
	for (auto oPair : object_list) {
		Serialize(oPair.second);
	}
}

void ObjectsManager::Serialize(DataTypes::LWOOBJID objID) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject && gameObject != nullptr && !gameObject->GetIsServerOnly())
		Serialize(gameObject);
}

void ObjectsManager::Serialize(Entity::GameObject * object) {
	if(!object->GetIsServerOnly())
		RM->SignalSerializeNeeded(object, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ObjectsManager::Destruct(DataTypes::LWOOBJID objID) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject != nullptr && !gameObject->GetIsServerOnly())
		Destruct(gameObject);
}

void ObjectsManager::Destruct(Entity::GameObject * object) {
	if(!object->GetIsServerOnly())
		RM->Destruct(object, UNASSIGNED_SYSTEM_ADDRESS, true);
	object_garbage.push_back(object->GetObjectID());
}

void ObjectsManager::OnUpdate() {
	// Call update
	for (auto it = object_list.begin(); it != object_list.end(); ++it) {
		if (it->second->GetObjectID() == it->first && it->second != nullptr)
			it->second->Update();
	}

	// Check for garbage
	for (auto it = object_garbage.begin(); it != object_garbage.end(); ++it) {
		object_list.erase(*it);
	}
	object_garbage.clear();

	// Check dirty
	for (auto oPair : object_list)
		if(oPair.second != nullptr)
			if (oPair.second->IsObjectDirty())
				if(oPair.second)
					if (!oPair.second->GetIsServerOnly())
						Serialize(oPair.second);
}

void ObjectsManager::OnPhysicsUpdate() {
	// Call update
	for (auto oPair : object_list) {
		if (oPair.second != nullptr && oPair.second) {
			if (std::find(object_garbage.begin(), object_garbage.end(), oPair.first) == object_garbage.end()) {
				oPair.second->PhysicUpdate();
			}
		}
	}
}
