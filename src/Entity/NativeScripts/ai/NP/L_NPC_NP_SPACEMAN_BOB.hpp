#ifndef __ENTITY__NATIVESCRIPTS__AI__NP__L_NPC_SPACEMAN_BOB_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__NP__L_NPC_SPACEMAN_BOB_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

#include "Database/Database.hpp"

class NATIVESCRIPT__AI__NP__L_NPC_SPACEMAN_BOB : public NativeScript {
	void onMissionDialogueOK(Entity::GameObject* self, GM::MissionDialogueOK msg) {
		if (msg.missionID == 173 && msg.bIsComplete) {
			// TODO SetImagination(6)
			// TODO UpdateMissionTask{taskType = "complete", value = 664, value2 = 1, target = self}

			Entity::GameObject* player = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.responder);

			DatabaseModels::MissionModel model;
			if (!Database::HasMission(self->GetZoneInstance()->GetDBConnection(), msg.responder.getPureID(), 664)) {
				model = Database::AddMission(self->GetZoneInstance()->GetDBConnection(), msg.responder.getPureID(), 664);
				player->SetImagination(6);
			}
			else {
				model = Database::GetMission(self->GetZoneInstance()->GetDBConnection(), msg.responder.getPureID(), 664);
			}


			MissionManager::LaunchTaskEvent(EMissionTask::SCRIPT, self, player->GetObjectID());

			GM::NotifyClientFlagChange flagGM;
			flagGM.bFlag = true;
			flagGM.iFlagID = 66;
			GameMessages::Send(player, player->GetObjectID(), flagGM);
		}
	}
};
#endif 