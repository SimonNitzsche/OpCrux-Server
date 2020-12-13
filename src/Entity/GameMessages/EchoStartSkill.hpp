#ifndef __ENTITY__GM__EchoStartSkill_HPP__
#define __ENTITY__GM__EchoStartSkill_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct EchoStartSkill : GMBase {
		bool bUsedMouse = false;
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
			return Enums::EGameMessageID::SERVER_START_SKILL;
		}

		EchoStartSkill() : GMBase() {}
		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bUsedMouse, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fCasterLatency, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, iCastType, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, lastClickedPosit, DataTypes::Vector3());
			GM_VAR_SERIALIZE(bs, optionalOriginatorID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, optionalTargetID, std::uint64_t(0));
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, originatorRot, DataTypes::Quaternion());
			GM_VAR_SERIALIZE_STRING(bs, sBitStream);
			GM_VAR_SERIALIZE(bs, skillID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, uiSkillHandle, 0);
		}
	};
}

#endif