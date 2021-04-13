#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__GENERAL__L_POI_MISSION_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__GENERAL__L_POI_MISSION_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"
#include "GameCache/MissionTasks.hpp"

class NATIVESCRIPT__02SERVER__MAP__GENERAL__L_POI_MISSION : public NativeScript {

	std::u16string POI;
	uint32_t MissionID;

public:

	void onStartup(Entity::GameObject* self) {
		POI = self->GetVar(u"POI").operator std::u16string();

		print("Loaded POI: " + StringUtils::to_string(POI));
		MissionID = CacheMissionTasks::GetID(CacheMissionTasks::getRowByTargetGroup(StringUtils::to_string(POI)));
		print("Loaded POI Mission: " + std::to_string(MissionID));
	}

	void onCollisionPhantom(Entity::GameObject* self, Entity::GameObject* other) {
		print("collided with POI " + StringUtils::to_string(POI));
		if (other->GetLOT() != 1) return;

		DatabaseModels::MissionModel mis;
		if (Database::HasMission(other->GetZoneInstance()->GetDBConnection(), other->GetObjectID().getPureID(), MissionID)) {
			mis = Database::GetMission(other->GetZoneInstance()->GetDBConnection(), other->GetObjectID().getPureID(), MissionID);
		}
		else {
			mis = Database::AddMission(other->GetZoneInstance()->GetDBConnection(), other->GetObjectID().getPureID(), MissionID);
		}

		if (mis.state == 8) return;

		mis.state = 8;

		GM::NotifyMission msg;
		msg.missionID = mis.missionID;
		msg.sendingRewards = false;
		msg.missionState = 2;
		GameMessages::Send(other, other->GetObjectID(), msg);
		msg.missionState = 4;
		GameMessages::Send(other, other->GetObjectID(), msg);

		msg.missionState = 0;
		msg.sendingRewards = true;
		GameMessages::Send(other, other->GetObjectID(), msg);

		Database::UpdateMission(other->GetZoneInstance()->GetDBConnection(), mis);
		MissionManager::SendMissionRewards(other, mis);

		msg.missionState = mis.state;
		msg.sendingRewards = false;
		GameMessages::Send(other, other->GetObjectID(), msg);
	}

};
#endif 