#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORAREAOFEFFECT_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORAREAOFEFFECT_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorAreaOfEffect : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::uint32_t numberOfTargets;
		bs->Read(numberOfTargets);
		std::list<std::uint64_t> targets;
		for (int i = 0; i < numberOfTargets; ++i) {
			std::uint64_t target;
			bs->Read(target);
			targets.push_back(target);
		}

		auto action = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");

		for (auto target : targets) {
			comp->SetTarget(target);
			StartUnCast(comp, action, bs);
		}
	}

	void Cast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {

		auto radius = CacheBehaviorParameter::GetParameterValue(behaviorID, "radius");
		auto max_targets = CacheBehaviorParameter::GetParameterValue(behaviorID, "max targets");

		auto objL = comp->owner->GetZoneInstance()->objectsManager->GetObjectsInRange(comp->owner->GetPosition(), radius, max_targets);

		bs->Write<std::uint32_t>(objL.size());
		for (auto obj : objL) {
			bs->Write(obj->GetObjectID());
		}

		auto action = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");

		for (auto obj : objL) {
			comp->SetTarget(obj->GetObjectID());
			StartCast(comp, action, bs);
		}

	}
};
#endif