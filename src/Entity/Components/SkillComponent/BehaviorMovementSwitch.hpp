#ifndef __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORMOVEMENTSWITCH_HPP__
#define __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORMOVEMENTSWITCH_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorMovementSwitch : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::uint32_t movementType; bs->Read(movementType);

		static const std::string switchTypes[]{ "moving_action", "ground_action", "jump_action", "falling_action", "double_jump_action", "air_action", "jetpack_action" };
		if (movementType <= 6) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, switchTypes[movementType]);
			StartUnCast(comp, nextID, bs);
		}
	}
};
#endif