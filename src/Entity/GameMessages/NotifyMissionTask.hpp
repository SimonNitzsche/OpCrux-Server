#ifndef __ENTITY__GM__NotifyMissionTask_HPP__
#define __ENTITY__GM__NotifyMissionTask_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyMissionTask : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_MISSION_TASK;
		}

		std::int32_t missionID;
		std::int32_t taskMask;
		std::list<std::float_t> updates;

		NotifyMissionTask() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, missionID);
			GM_VAR_SERIALIZE(bs, taskMask);
			bs->Write<std::uint8_t>(updates.size());
			for (auto it = updates.begin(); it != updates.end(); ++it)
				bs->Write<std::float_t>(*it);
		}
	};
}

#endif