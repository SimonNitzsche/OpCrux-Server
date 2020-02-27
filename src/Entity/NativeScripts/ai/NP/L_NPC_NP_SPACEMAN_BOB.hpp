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
			player->SetImagination(6);

			Database::MissionModel model;
			if (!Database::HasMission(msg.responder.getPureID(), 664)) {
				model = Database::AddMission(msg.responder.getPureID(), 664);
			}
			else {
				model = Database::GetMission(msg.responder.getPureID(), 664);
			}

			model.state = 2;

			GM::NotifyMissionTask taskNotify;
			taskNotify.missionID = 664;
			taskNotify.taskMask = 2;
			taskNotify.updates = {1.0f};
			GameMessages::Send(player, player->GetObjectID(), taskNotify);

			GM::NotifyMission misNotify;
			misNotify.missionID = model.missionID;
			misNotify.sendingRewards = true;
			misNotify.missionState = 0;
			GameMessages::Send(player, player->GetObjectID(), misNotify);

			model.state = 8;
			model.progress = "1";
			misNotify.missionState = model.state;
			Database::UpdateMission(model);

			misNotify.sendingRewards = true;
			GameMessages::Send(player, player->GetObjectID(), misNotify);
		}
	}
};
#endif 