#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORBASICATTACK_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORBASICATTACK_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorBasicAttack : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		bs->AlignReadToByteBoundary();
		bs->IgnoreBits(16);

		bool unknown1, unknown2, unknown3;
		bs->Read(unknown1);
		bs->Read(unknown2);
		bs->Read(unknown3);

		std::uint32_t unknown4;
		bs->Read(unknown4);

		std::uint32_t damage;
		bs->Read(damage);

		comp->DoDamageOnTarget(damage);

		bool success;
		bs->Read(success);

		if (success) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "on_success");
			StartUnCast(comp, nextID, bs);
		}
	}
};
#endif