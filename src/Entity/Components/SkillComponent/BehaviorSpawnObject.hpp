#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSPAWNOBJECT_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSPAWNOBJECT_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorSpawnObject : AbstractAggregateBehavior {
	bool unknownBit = false;

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::float_t lotID = CacheBehaviorParameter::GetParameterValue(behaviorID, "LOT_ID");
		
		if (lotID == -1) return;
		Entity::GameObject* spawnedObject = new Entity::GameObject(comp->owner->GetZoneInstance(), lotID);

		spawnedObject->SetObjectID(DataTypes::LWOOBJID((1ULL << 58) + 104120439353844ULL + comp->owner->GetZoneInstance()->spawnedObjectIDCounter++));

		spawnedObject->SetParent(comp->owner);

		spawnedObject->SetPosition(comp->owner->GetPosition());
		spawnedObject->SetRotation(comp->owner->GetRotation());

		spawnedObject->PopulateFromLDF({});
		spawnedObject->Finish();

		spawnedObject->GetZoneInstance()->objectsManager->RegisterObject(spawnedObject);
		spawnedObject->GetZoneInstance()->objectsManager->Construct(spawnedObject);

	}
};
#endif