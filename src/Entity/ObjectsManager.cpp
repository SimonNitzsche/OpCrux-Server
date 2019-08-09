#include "Entity/ObjectsManager.hpp"

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

void ObjectsManager::Construct(DataTypes::LWOOBJID objID) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject != nullptr)
		Construct(gameObject);
}

void ObjectsManager::Construct(Entity::GameObject * object) {
	RM->Construct(object, false, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ObjectsManager::Serialize() {
	for (auto oPair : object_list) {
		Serialize(oPair.second);
	}
}

void ObjectsManager::Serialize(DataTypes::LWOOBJID objID) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject != nullptr)
		Serialize(gameObject);
}

void ObjectsManager::Serialize(Entity::GameObject * object) {
	RM->SignalSerializeNeeded(object, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ObjectsManager::Destruct(DataTypes::LWOOBJID objID) {
	Entity::GameObject * gameObject = GetObjectByID(objID);
	if (gameObject != nullptr)
		Destruct(gameObject);
}

void ObjectsManager::Destruct(Entity::GameObject * object) {
	RM->Destruct(object, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void ObjectsManager::OnUpdate() {
	// Call update
	for (auto oPair : object_list)
		oPair.second->Update();

	// Check dirty
	for (auto oPair : object_list)
		if (oPair.second->IsObjectDirty())
			Serialize(oPair.second);
}