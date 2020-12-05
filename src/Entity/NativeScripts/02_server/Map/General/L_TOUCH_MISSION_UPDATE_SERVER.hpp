#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__GENERAL__L_TOUCH_MISSION_UPDATE_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__GENERAL__L_TOUCH_MISSION_UPDATE_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"

class NATIVESCRIPT__02SERVER__MAP__GENERAL__L_TOUCH_MISSION_UPDATE_SERVER : public NativeScript {

	std::int32_t TouchCompleteID;

public:

	void onStartup(Entity::GameObject* self) {
		TouchCompleteID = self->GetVar(u"TouchCompleteID").operator std::int32_t();

	}

	void onCollisionPhantom(Entity::GameObject* self, Entity::GameObject* other) {
		if (other->GetLOT() != 1) return;
		print("Complete " + std::to_string(TouchCompleteID));

		auto model = Database::GetMission(self->GetZoneInstance()->GetDBConnection(), other->GetObjectID().getPureID(), 664);
		if(model.state == 2)
			MissionManager::LaunchTaskEvent(Enums::EMissionTask::SCRIPT, self, other->GetObjectID());
	}

};
#endif 