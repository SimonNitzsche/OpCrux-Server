#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORFORCEMOVEMENT_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORFORCEMOVEMENT_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorForceMovement : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		
		std::int32_t actionToHandle = 0;
		while (true) {
			
			actionToHandle = CacheBehaviorParameter::GetParameterValue(behaviorID, "hit_action");
			if (actionToHandle > 0) break;
			
			actionToHandle = CacheBehaviorParameter::GetParameterValue(behaviorID, "hit_action_enemy");
			if (actionToHandle > 0) break;
			
			actionToHandle = CacheBehaviorParameter::GetParameterValue(behaviorID, "hit_action_faction");
			break;
		}

		if (actionToHandle > 0) {
			std::uint32_t behaviorHandle;
			bs->Read(behaviorHandle);
		
			comp->AddBehaviorHandle(behaviorHandle, actionToHandle);
		}
	}
};
#endif