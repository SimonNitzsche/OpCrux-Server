#ifndef __ENTITY__GM__RespondToMission_HPP__
#define __ENTITY__GM__RespondToMission_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RespondToMission : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_RESPOND_TO_MISSION;
		}

		std::int32_t missionID;
		DataTypes::LWOOBJID playerID;
		DataTypes::LWOOBJID receiver;
		std::int32_t rewardItem = -1;

		RespondToMission() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, missionID);
			GM_VAR_DESERIALIZE(bs, playerID);
			GM_VAR_DESERIALIZE(bs, receiver);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, rewardItem, -1);
		}
	};
}

#endif