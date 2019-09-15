#ifndef __ENTITY__GM__EmotePlayed_HPP__
#define __ENTITY__GM__EmotePlayed_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct EmotePlayed : GMBase {
		std::int32_t emoteID;
		DataTypes::LWOOBJID targetID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_EMOTE_PLAYED;
		}

		

		EmotePlayed() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, emoteID);
			GM_VAR_SERIALIZE(bs, targetID);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), *this);
		}
	};
}

#endif