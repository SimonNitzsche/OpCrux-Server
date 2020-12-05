#ifndef __ENTITY__GM__RequestSmashPlayer_HPP__
#define __ENTITY__GM__RequestSmashPlayer_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RequestSmashPlayer : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_SMASH_PLAYER;
		}

		RequestSmashPlayer() {}
		void Deserialize(RakNet::BitStream * bs) {
			
		}

	};
}

#endif