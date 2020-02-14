#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORALTERCHAINDELAY_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORALTERCHAINDELAY_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorAlterChainDelay : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {

	}
};
#endif