#ifndef __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORKNOCKBACK_HPP__
#define __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORKNOCKBACK_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorKnockback : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		// INTERUPT

		bool unknown1;

		bs->Read(unknown1);

		// TODO
	}
};
#endif