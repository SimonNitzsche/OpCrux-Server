#ifndef __ENTITY__GM__NotifyMission_HPP__
#define __ENTITY__GM__NotifyMission_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyMission : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_MISSION;
		}

		std::int32_t missionID;
		std::int32_t missionState;
		bool sendingRewards = false;

		NotifyMission() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, missionID);
			GM_VAR_SERIALIZE(bs, missionState);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, sendingRewards, false);
		}
	};
}

#endif