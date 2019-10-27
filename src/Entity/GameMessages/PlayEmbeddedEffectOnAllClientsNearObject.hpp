#ifndef __ENTITY__GM__PlayEmbeddedEffectOnAllClientsNearObject_HPP__
#define __ENTITY__GM__PlayEmbeddedEffectOnAllClientsNearObject_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayEmbeddedEffectOnAllClientsNearObject : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAY_EMBEDDED_EFFECT_ON_ALL_NEARBY_CLIENTS;
		}

		std::wstring effectName = L"";
		DataTypes::LWOOBJID fromObjectID = 0;
		float radius = 1.0f;

		PlayEmbeddedEffectOnAllClientsNearObject() : GMBase() {}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WSTRING(bs, effectName);
			GM_VAR_SERIALIZE(bs, fromObjectID);
			GM_VAR_SERIALIZE(bs, radius);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
		
		}
	};
}

#endif