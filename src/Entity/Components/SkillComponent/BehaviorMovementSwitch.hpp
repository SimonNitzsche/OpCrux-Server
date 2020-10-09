#ifndef __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORMOVEMENTSWITCH_HPP__
#define __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORMOVEMENTSWITCH_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorMovementSwitch : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::uint32_t movementType; bs->Read(movementType);

		static const std::string switchTypes[]{ /* 0, 1 */ "jump_action", "air_action", "double_jump_action", "falling_action", "jetpack_action" };

		if (2 <= movementType && movementType <= 6) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, switchTypes[movementType - 2]);
			StartUnCast(comp, nextID, bs);
			return;
		}

		std::int32_t movingAction = CacheBehaviorParameter::GetParameterValue(behaviorID, "moving_action");
		std::int32_t groundAction = CacheBehaviorParameter::GetParameterValue(behaviorID, "ground_action");

		if (movingAction == groundAction) {
			std::int32_t nextID = groundAction;
			StartUnCast(comp, nextID, bs);
		}
		else {

			// Note: there is some code in the client at this position,
			// that may influence parsing at this section

			std::int32_t nextID = groundAction;
			StartUnCast(comp, nextID, bs);
		}
	}
};
#endif