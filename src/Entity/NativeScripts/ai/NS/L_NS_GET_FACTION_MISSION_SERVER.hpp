#ifndef __ENTITY__NATIVESCRIPTS__AI__NS__L_NS_GET_FACTION_MISSION_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__NS__L_NS_GET_FACTION_MISSION_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"
#include "Entity/Components/SpawnerComponent.hpp"

class NATIVESCRIPT__AI__NS__L_NS_GET_FACTION_MISSION_SERVER : public NativeScript {

	static const auto misID = 474; // -- Join a faction mission

public:

	
	void onRespondToMission(Entity::GameObject* self, GM::RespondToMission msg) {
		if (msg.missionID != misID) return;

		auto player = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.playerID);
		auto celebrationID = -1;
		auto iFactionNum = -1;

		// if the player is not on the mission or not completed the mission then set the icons on the faction npc's
		if (msg.rewardItem != 0 && msg.rewardItem != -1) {
			std::list<std::int32_t> tUpdateMissions = {};
		
			// set the players new Factionand sets the corrisponding celebration
			if (msg.rewardItem == 6980) { // -- Venture <-> 7090 ************************************************
				iFactionNum = 1;
				tUpdateMissions = { 555, 556, 778 }; // --setup the Venture Achievements that need to be completed
				celebrationID = 14;
			}
			else if (msg.rewardItem == 6979) { // -- Assembly <-> 7091 ***********************************************
				iFactionNum = 2;
				tUpdateMissions = { 544, 545, 778 }; // --setup the Assembly Achievements that need to be completed
				celebrationID = 15;
			}
			else if (msg.rewardItem == 6981) { // -- Paradox <-> 7092 ************************************************
				iFactionNum = 3;
				tUpdateMissions = { 577, 578, 778 }; // --setup the Paradox Achievements that need to be completed
				celebrationID = 16;
			}
			else if (msg.rewardItem == 6978) { // -- Sentinel <-> 7093 ***********************************************
				iFactionNum = 4;
				tUpdateMissions = { 566, 567, 778 }; // --setup the Sentinel Achievements that need to be completed
				celebrationID = 17;
			}

			// set player kit faction
			if (iFactionNum != -1) {
				{GM::SetFlag sfm; sfm.bFlag = true; sfm.iFlagID = 45 + iFactionNum; player->OnMessage(player, sfm.GetID(), &sfm); }
			}

			// play the celebration
			if (celebrationID != -1) {
				{GM::StartCelebrationEffect scem; scem.celebrationID = celebrationID; GameMessages::Send(player, player->GetObjectID(), scem); }
			}

			// add and complete the achievements
			for (auto v : tUpdateMissions) {
				DatabaseModels::MissionModel mis;
				if (Database::HasMission(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID().getPureID(), v)) {
					mis = Database::GetMission(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID().getPureID(), v);
				}
				else {
					mis = Database::AddMission(player->GetZoneInstance()->GetDBConnection(), player->GetObjectID().getPureID(), v);
				}

				if (mis.state == 8) continue;

				mis.state = 8;

				GM::NotifyMission msg;
				msg.missionID = mis.missionID;
				msg.sendingRewards = false;
				msg.missionState = 2;
				GameMessages::Send(player, player->GetObjectID(), msg);
				msg.missionState = 4;
				GameMessages::Send(player, player->GetObjectID(), msg);

				msg.missionState = 0;
				msg.sendingRewards = true;
				GameMessages::Send(player, player->GetObjectID(), msg);


				// Send rewards
				Database::UpdateMission(player->GetZoneInstance()->GetDBConnection(), mis);
				MissionManager::SendMissionRewards(player, mis);

				msg.missionState = mis.state;
				msg.sendingRewards = false;
				GameMessages::Send(player->GetZoneInstance(), player->GetZoneInstance()->sessionManager.GetSession(player->GetObjectID())->systemAddress, player->GetObjectID(), msg);
			}
		}
	}

	

};
#endif 