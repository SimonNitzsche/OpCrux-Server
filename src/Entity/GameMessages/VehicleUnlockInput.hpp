#ifndef __ENTITY__GM__VehicleUnlockInput_HPP__
#define __ENTITY__GM__VehicleUnlockInput_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct VehicleUnlockInput : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_VEHICLE_UNLOCK_INPUT;
		}

		bool bLockWheels = true;

		VehicleUnlockInput() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bLockWheels);
		}
	};
}

#endif