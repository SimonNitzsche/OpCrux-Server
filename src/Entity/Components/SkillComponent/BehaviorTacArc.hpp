#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORTACARC_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORTACARC_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

#include "Entity/Components/SkillComponent.hpp"

struct BehaviorTacArc : AbstractAggregateBehavior {

	bool hitSomething = false;
	bool bBlocked = false;
	std::vector<DataTypes::LWOOBJID> targets = {};

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		SkillStackParameters ssParams = comp->GetParameters();
		bool usePickedTarget = (1.0f == CacheBehaviorParameter::GetParameterValue(behaviorID, "use_picked_target"));
	
		if(usePickedTarget && ssParams.optionalTargetID != 0ULL) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");
			StartUnCast(comp, nextID, bs);
		}
		else {
			bs->Read(hitSomething);

			bool checkEnv = (1.0f == CacheBehaviorParameter::GetParameterValue(behaviorID, "check_env"));
			if (checkEnv) {
				bs->Read(bBlocked);

				if (bBlocked) {
					std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "blocked_action");
					StartUnCast(comp, nextID, bs);
					return;
				}
			}

			if (hitSomething) {
				
				targets.clear();
				std::uint32_t tmpNumberOfTargets; bs->Read(tmpNumberOfTargets);
				targets.reserve(tmpNumberOfTargets);
				for (int i = 0; i < tmpNumberOfTargets; ++i) {
					std::uint64_t tmpObjectID; bs->Read(tmpObjectID);
					targets.push_back(tmpObjectID);
				}

				for (int i = 0; i < tmpNumberOfTargets; ++i) {
					std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");
					comp->SetTarget(targets.at(i));
					StartUnCast(comp, nextID, bs);
				}
			}
			else {
				std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "miss action");
				StartUnCast(comp, nextID, bs);
			}
		
		}
	}
};
#endif