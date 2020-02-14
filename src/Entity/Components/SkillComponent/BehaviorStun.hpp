#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSTUN_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORSTUN_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorStun : AbstractAggregateBehavior {
	bool unknownBit = false;

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::float_t stun_caster = CacheBehaviorParameter::GetParameterValue(behaviorID, "stun_caster");
		if (stun_caster == 0.0f)
			bs->Read(unknownBit);
	}
};
#endif