#ifndef __ENTITY__GM__EchoSyncSkill_HPP__
#define __ENTITY__GM__EchoSyncSkill_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct EchoSyncSkill : GMBase {
		bool bDone = false;
		std::string sBitStream;
		std::uint32_t uiBehaviorHandle;
		std::uint32_t uiSkillHandle;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SYNC_SKILL;
		}

		EchoSyncSkill() : GMBase() {}
		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bDone, false);
			GM_VAR_SERIALIZE_STRING(bs, sBitStream);
			GM_VAR_SERIALIZE(bs, uiBehaviorHandle);
			GM_VAR_SERIALIZE(bs, uiSkillHandle)
		}

	};
}


#endif