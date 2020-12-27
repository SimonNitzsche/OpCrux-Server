#ifndef __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORBASICATTACK_HPP__
#define __ENTITY__COMPONENTS__SKILLSYSTEM__BEHAVIORBASICATTACK_HPP__
#include "Entity/Components/SkillComponent/AbstractAggregateBehavior.hpp"

struct BehaviorBasicAttack : AbstractAggregateBehavior {

	void UnCast(SkillComponent* comp, std::int32_t behaviorID, RakNet::BitStream* bs) {
		bs->AlignReadToByteBoundary();
		std::uint16_t allocated_bits;
		bs->Read(allocated_bits);

		bool blocked, immune, success, died = false;
		bs->Read(blocked);
		if (blocked) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "on_fail_blocked");
			StartUnCast(comp, nextID, bs);
			return;
		}

		bs->Read(immune);
		if (immune) {
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "on_fail_immune");
			StartUnCast(comp, nextID, bs);
			return;
		}

		bs->Read(success);
		std::uint32_t unknown_always_0, damage;
		if (success) {
			bs->Read(unknown_always_0);
			bs->Read(damage);
			bs->Read(died);
		}

		std::uint8_t success_state;
		bs->Read(success_state);

		if (success_state == 1) {
			comp->DoDamageOnTarget(std::int32_t(damage));
			std::int32_t nextID = CacheBehaviorParameter::GetParameterValue(behaviorID, "on_success");
			StartUnCast(comp, nextID, bs);
		}
	}
};
#endif