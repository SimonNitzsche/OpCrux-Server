#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORCHAIN_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORCHAIN_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorChain : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		std::uint32_t chainIndex; bs->Read(chainIndex);


		std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "behavior " + std::to_string(chainIndex));
		StartUnCast(comp, nextID, bs);
	}
};
#endif