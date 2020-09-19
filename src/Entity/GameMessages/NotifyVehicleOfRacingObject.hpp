#ifndef __ENTITY__GM__NotifyVehicleOfRacingObject_HPP__
#define __ENTITY__GM__NotifyVehicleOfRacingObject_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct NotifyVehicleOfRacingObject : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_VEHICLE_OF_RACING_OBJECT;
		}

		DataTypes::LWOOBJID racingObjectID;

		NotifyVehicleOfRacingObject() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, racingObjectID, DataTypes::LWOOBJID());
		}
	};
}

#endif