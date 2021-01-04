#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSPAWNQUICKBUILD_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSPAWNQUICKBUILD_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorSpawnQuickbuild : AbstractAggregateBehavior {
	bool unknownBit = false;

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::float_t lotID = CacheBehaviorParameter::GetParameterValue(behaviorID, "LOT_ID");
		std::float_t offsetX = CacheBehaviorParameter::GetParameterValue(behaviorID, "offsetX");
		std::float_t offsetY = CacheBehaviorParameter::GetParameterValue(behaviorID, "offsetY");
		std::float_t offsetZ = CacheBehaviorParameter::GetParameterValue(behaviorID, "offsetZ");

		if (lotID == -1) return;
		Entity::GameObject* spawnedObject = new Entity::GameObject(comp->owner->GetZoneInstance(), lotID);

		spawnedObject->SetObjectID(comp->owner->GetZoneInstance()->objectsManager->GenerateSpawnedID());

		spawnedObject->SetParent(comp->owner);

		spawnedObject->SetPosition(comp->owner->GetPosition());
		spawnedObject->SetRotation(comp->owner->GetRotation());

		auto spawnPos = spawnedObject->GetPosition();
		spawnPos += DataTypes::Vector3(offsetX, offsetY, offsetZ);

		LDFCollection collection = {
			LDF_COLLECTION_INIT_ENTRY(u"rebuild_activators", StringUtils::to_u16string(std::to_string(spawnPos.x) + (char)(0x1f) + std::to_string(spawnPos.y) + (char)(0x1f) + std::to_string(spawnPos.z)))
		};

		spawnedObject->PopulateFromLDF(&collection);
		spawnedObject->Finish();

		spawnedObject->GetZoneInstance()->objectsManager->RegisterObject(spawnedObject);
		spawnedObject->GetZoneInstance()->objectsManager->Construct(spawnedObject);

	}
};
#endif