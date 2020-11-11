#ifndef __ENTITY__GM__PlayEmote_HPP__
#define __ENTITY__GM__PlayEmote_HPP__
#include "Entity/GameMessages.hpp"

#include "EmotePlayed.hpp"

namespace GM {
	struct PlayEmote : GMBase {
		std::int32_t emoteID;
		DataTypes::LWOOBJID targetID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_PLAY_EMOTE;
		}

		PlayEmote() {}
		void Deserialize(RakNet::BitStream * bs) {
			GM_VAR_DESERIALIZE(bs, emoteID);
			GM_VAR_DESERIALIZE(bs, targetID);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
		}
	};
}

#endif