#ifndef __ENTITY__GM__StopFXEffect_HPP__
#define __ENTITY__GM__StopFXEffect_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StopFXEffect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_STOP_FX;
		}

		bool bKillImmediate = false;
		std::string name;
		

		StopFXEffect() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, bKillImmediate);
			GM_VAR_SERIALIZE_STRING(bs, name);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), *this);
		}
	};
}

#endif