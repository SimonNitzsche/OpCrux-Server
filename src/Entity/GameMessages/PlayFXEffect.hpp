#ifndef __ENTITY__GM__PlayFXEffect_HPP__
#define __ENTITY__GM__PlayFXEffect_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayFXEffect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAY_FX;
		}

		std::int32_t effectID = -1;
		std::wstring effectType = L"";
		float fScale = 1.0f;
		std::string name;
		float priority = 1.0f;
		DataTypes::LWOOBJID secondary = 0;
		bool serialize = true;

		PlayFXEffect() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, effectID, -1);
			GM_VAR_SERIALIZE_WSTRING(bs, effectType);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, fScale, 1.0f);
			GM_VAR_SERIALIZE_STRING(bs, name);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, priority, 1.0f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, secondary, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, serialize, true);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), *this);
		}
	};
}

#endif