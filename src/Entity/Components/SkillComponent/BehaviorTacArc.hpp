#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORTACARC_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORTACARC_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

#include "Entity/Components/SkillComponent.hpp"

struct BehaviorTacArc : AbstractAggregateBehavior {

	bool hitSomething = false;
	bool unknownEnvironmentBit = false;
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
			if (hitSomething) {
				bool checkEnv = (1.0f == CacheBehaviorParameter::GetParameterValue(behaviorID, "check_env"));
				if (checkEnv) {
					bs->Read(unknownEnvironmentBit);
				}

				targets.clear();
				std::uint32_t tmpNumberOfTargets; bs->Read(tmpNumberOfTargets);
				targets.reserve(tmpNumberOfTargets);
				for (int i = 0; i < tmpNumberOfTargets; ++i) {
					std::uint64_t tmpObjectID; bs->Read(tmpObjectID);
					targets.push_back(tmpObjectID);
				}

				for (int i = 0; i < tmpNumberOfTargets; ++i) {
					std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");
					StartUnCast(comp, nextID, bs);
				}
			}
			else {
				std::int32_t blockedAction = CacheBehaviorParameter::GetParameterValue(behaviorID, "blocked_action");
				bool bBlockedAction = (0.0f <= blockedAction);
				if (bBlockedAction) {
					bool blocked; bs->Read(blocked);
					if (blocked) {
						std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "blocked_action");
						StartUnCast(comp, nextID, bs);
					}
					else {
						std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "miss action");
						StartUnCast(comp, nextID, bs);
					}
				}
				else {
					std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "miss action");
					StartUnCast(comp, nextID, bs);
				}
			}
		}

		
	}
};
#endif