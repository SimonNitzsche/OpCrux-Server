#ifndef __ENTITY__GM__RespondToMission_HPP__
#define __ENTITY__GM__RespondToMission_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RespondToMission : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_RESPOND_TO_MISSION;
		}

		std::int32_t missionID;
		std::int32_t missionState;
		bool sendingRewards = false;

		RespondToMission() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, missionID);
			GM_VAR_SERIALIZE(bs, missionState);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sendingRewards, false);
		}
	};
}

#endif