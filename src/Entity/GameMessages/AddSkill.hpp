#ifndef __ENTITY__GM__AddSkill_HPP__
#define __ENTITY__GM__AddSkill_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/EFailReason.hpp"
#include "DataTypes/LWOOBJID.hpp"

namespace GM {
	struct AddSkill : GMBase {

		inline constexpr static ::Enums::EGameMessageID GetID() {
			return ::Enums::EGameMessageID::SERVER_ADD_SKILL;
		}

		std::int32_t AICombatWeight = 0;
		bool bFromSkillSet = false;
		std::int32_t castType = 0;
		std::float_t fTimeSecs = -1.0f;
		std::int32_t iTimesCanCast = -1;
		std::int32_t skillID = 0;
		std::int32_t slotID = -1.0f;
		bool temporary = true;

		AddSkill() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, AICombatWeight, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bFromSkillSet, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, castType, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fTimeSecs, -1.0f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, iTimesCanCast, -1);
			GM_VAR_SERIALIZE(bs, skillID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, slotID, -1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, temporary, true);

		}
	};
}

#endif