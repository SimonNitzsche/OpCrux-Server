#ifndef __ENTITY__GM__REMOVESKILL_HPP__
#define __ENTITY__GM__REMOVESKILL_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RemoveSkill : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_REMOVE_SKILL;
		}

		RemoveSkill() {}

		bool bFromSkillSet = false;
		uint32_t skillID;

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bFromSkillSet, false);
			GM_VAR_SERIALIZE(bs, skillID);
		}
	};
}

#endif