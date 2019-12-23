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

			Database::MissionModel model;
			if (!Database::HasMission(msg.responder.getPureID(), 664)) {
				model = Database::AddMission(msg.responder.getPureID(), 664);
			}
			else {
				model = Database::GetMission(msg.responder.getPureID(), 664);
			}
			model.state = 8;
			model.progress = 1;
			Database::UpdateMission(model);
		}
	}
};
#endif 