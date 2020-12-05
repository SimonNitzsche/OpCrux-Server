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

			GM::NotifyMissionTask taskNotify;
			taskNotify.missionID = 664;
			taskNotify.taskMask = 2;
			taskNotify.updates = { 1.0f };
			GameMessages::Send(player, player->GetObjectID(), taskNotify);

			model.state = 2;

			GM::NotifyMission misNotify;
			misNotify.missionID = model.missionID;
			misNotify.sendingRewards = true;
			misNotify.missionState = 0;
			GameMessages::Send(player, player->GetObjectID(), misNotify);

			model.state = 8;
			model.progress = "1";
			misNotify.missionState = model.state;
			Database::UpdateMission(self->GetZoneInstance()->GetDBConnection(), model);

			misNotify.sendingRewards = true;
			GameMessages::Send(player, player->GetObjectID(), misNotify);

			GM::NotifyClientFlagChange flagGM;
			flagGM.bFlag = true;
			flagGM.iFlagID = 66;
			GameMessages::Send(player, player->GetObjectID(), flagGM);
		}
	}
};
#endif 