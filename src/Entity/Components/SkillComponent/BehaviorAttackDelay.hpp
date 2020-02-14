#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORATTACKDELAY_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORATTACKDELAY_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorAttackDelay : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {

		std::int32_t num_intervals = CacheBehaviorParameter::GetParameterValue(behaviorID, "num_intervals");

		if (num_intervals <= 0) num_intervals = true;

		// num interval seems to affect the count, if it's false comment out following line
		// num_intervals = 1;

		std::int32_t actionToHandle = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");

		for (int i = 0; i < num_intervals; ++i) {
			std::uint32_t behaviorHandle;
			bs->Read(behaviorHandle);

			comp->AddBehaviorHandle(behaviorHandle, actionToHandle);
		}
	}
};
#endif