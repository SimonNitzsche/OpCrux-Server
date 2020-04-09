#ifndef __ENTITY__GM__SyncSkill_HPP__
#define __ENTITY__GM__SyncSkill_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SyncSkill : GMBase {
		bool bDone=false;
		std::string sBitStream;
		std::uint32_t uiBehaviorHandle;
		std::uint32_t uiSkillHandle;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_SYNC_SKILL;
		}

		SyncSkill() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bDone, false);
			GM_VAR_DESERIALIZE_STRING(bs, sBitStream);
			GM_VAR_DESERIALIZE(bs, uiBehaviorHandle);
			GM_VAR_DESERIALIZE(bs, uiSkillHandle)
		}

		inline void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered SyncSkill handle " + std::to_string(uiSkillHandle) + " with behaviorID " + std::to_string(uiBehaviorHandle));
			sender->OnSyncSkill(sender, *this);
		}
	};
}


#endif