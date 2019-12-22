#ifndef __ENTITY__GM__RequestLinkedMission_HPP__
#define __ENTITY__GM__RequestLinkedMission_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct RequestLinkedMission : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_LINKED_MISSION;
		}

		DataTypes::LWOOBJID playerID;
		std::int32_t missionID;
		bool bMissionOffered = false;

		RequestLinkedMission() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, playerID);
			GM_VAR_DESERIALIZE(bs, missionID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bMissionOffered, false);
		}

		inline void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("WRLD", sender->GetNameStr() + " requesting LinkedMission for missionID " + std::to_string(missionID) + " bMissionOffered " + (bMissionOffered?"true":"false"));
			
		}
	};
}

#endif