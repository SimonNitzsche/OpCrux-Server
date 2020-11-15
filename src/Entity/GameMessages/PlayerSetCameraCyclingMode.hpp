#ifndef __ENTITY__GM__PlayerSetCameraCyclingMode_HPP__
#define __ENTITY__GM__PlayerSetCameraCyclingMode_HPP__
#include "Entity/GameMessages.hpp"

#include "EmotePlayed.hpp"

namespace GM {
	struct PlayerSetCameraCyclingMode : GMBase {
		std::int32_t bAllowCyclingWhildeDeadOnly = true;
		std::int32_t cyclingMode = 0;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_PLAYER_CAMERA_CYCLING_MODE;
		}

		PlayerSetCameraCyclingMode() {}
		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bAllowCyclingWhildeDeadOnly, true);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, cyclingMode, 0);
		}

	};
}

#endif