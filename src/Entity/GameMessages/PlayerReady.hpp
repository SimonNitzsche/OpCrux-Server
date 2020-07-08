#ifndef __ENTITY__GM__PlayerReady_HPP__
#define __ENTITY__GM__PlayerReady_HPP__

#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayerReady : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_PLAYER_READY;
		}

		PlayerReady() : GMBase() {}
	};
}

#endif