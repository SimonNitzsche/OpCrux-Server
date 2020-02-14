#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORCHANGEORIENTATION_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORCHANGEORIENTATION_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorChangeOrientation : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "action");
		StartUnCast(comp, nextID, bs);
	}
};
#endif