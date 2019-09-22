#ifndef __ENTITY__GM__StartSkill_HPP__
#define __ENTITY__GM__StartSkill_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StartSkill : GMBase {
		bool bUsedMouse = false;
		DataTypes::LWOOBJID consumableItemID;
		std::float_t fCasterLatency = 0;
		std::int32_t iCastType = 0;
		DataTypes::Vector3 lastClickedPosit;
		DataTypes::LWOOBJID optionalOriginatorID;
		DataTypes::LWOOBJID optionalTargetID;
		DataTypes::Quaternion originatorRot;
		std::string sBitStream = "";
		std::uint32_t skillID = 0;
		std::uint32_t uiSkillHandle = 0;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_START_SKILL;
		}

		StartSkill() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bUsedMouse, false);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, consumableItemID, 0ULL);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, fCasterLatency, 0);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, iCastType, 0);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, lastClickedPosit, DataTypes::Vector3());
			GM_VAR_DESERIALIZE(bs, optionalOriginatorID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, optionalTargetID, 0ULL);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, originatorRot, DataTypes::Quaternion());
			GM_VAR_DESERIALIZE_STRING(bs, sBitStream);
			GM_VAR_DESERIALIZE(bs, skillID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, uiSkillHandle, 0);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered StartSkill handle " + std::to_string(uiSkillHandle) + " with skillID " + std::to_string(skillID));
			sender->OnStartSkill(*this);
		}
	};
}

#endif