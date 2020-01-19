#ifndef __ENTITY__GM__RacingPlayerLoaded_HPP__
#define __ENTITY__GM__RacingPlayerLoaded_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct RacingPlayerLoaded : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_RACING_PLAYER_LOADED;
		}

		DataTypes::LWOOBJID playerID;
		DataTypes::LWOOBJID vehicleID;

		RacingPlayerLoaded() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, playerID);
			GM_VAR_SERIALIZE(bs, vehicleID);
		}
	};
}

#endif