#ifndef __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORINTERRUPT_HPP__
#define __ENTITY__COMPONENT__SKILLSYSTEM__BEHAVIORINTERRUPT_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorInterrupt : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		// INTERUPT

		bool unknown1, unknown2, unknown3;

		// TODO
	}
};
#endif