#ifndef __ENTITY__GM__MissionDialogueOK_HPP__
#define __ENTITY__GM__MissionDialogueOK_HPP__
#include "Entity/GameMessages.hpp"
#include "Database/Database.hpp"

namespace GM {
	struct MissionDialogueOK : GMBase {
		bool bIsComplete;
		std::int32_t iMissionState;
		std::int32_t missionID;
		DataTypes::LWOOBJID responder;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MISSION_DIALOGUE_OK;
		}

		MissionDialogueOK() {}
		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, bIsComplete);
			GM_VAR_DESERIALIZE(bs, iMissionState);
			GM_VAR_DESERIALIZE(bs, missionID);
			GM_VAR_DESERIALIZE(bs, responder);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("WRLD", "Triggered MissionDialogueOK.");
			
			if (!Database::HasMission(sender->GetObjectID() & 0xFFFFFFFF, missionID)) {
				Database::AddMission(sender->GetObjectID() & 0xFFFFFFFF, missionID);
			}

			//Entity::GameObject* targetObject = sender->GetZoneInstance()->objectsManager->GetObjectByID(objectID);
			/*if (targetObject)
				targetObject->OnRequestUse(sender, this);
			else
				Logger::log("WRLD", "RequestUse: Unable to find object " + std::to_string(objectID), LogType::ERR);*/
		}
	};
}

#endif