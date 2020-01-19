#ifndef __ENTITY__NATIVESCRIPTS__AI__RACING__TRACK_GF__GF_RACE_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__RACING__TRACK_GF__GF_RACE_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

#include "Entity/NativeScripts/ai/RACING/RACE_IMAGINATION_SPAWNER.hpp"
#include "Entity/NativeScripts/OMIS.hpp"

class NATIVESCRIPT__AI__RACING__TRACK_GF__GF_RACE_SERVER : public NativeScript, public NATIVESCRIPT__AI__RACING__RACE_IMAGINATION_SPAWNER, public NATIVESCRIPT__OMIS {

public:

	void onStartup(Entity::GameObject* self) {
		// print("RACING: onStartup");


		// configure the racing control
		LDFCollection racingParams = {
			LDF_COLLECTION_INIT_ENTRY(u"GameType", "Racing"),
			LDF_COLLECTION_INIT_ENTRY(u"GameState", "Starting"),					// -- Do Not Change --
			LDF_COLLECTION_INIT_ENTRY(u"Number_Of_PlayersPerTeam", 6),				// INT ( Set the number of players on each team )
			LDF_COLLECTION_INIT_ENTRY(u"Minimum_Players_to_Start", 2),				// INT ( The min number of players to start game )
			LDF_COLLECTION_INIT_ENTRY(u"Minimum_Players_for_Group_Achievments", 3), // INT ( the minimum number of players required to get "group" achievements )
			
			// --- Game Object Lots ---
			LDF_COLLECTION_INIT_ENTRY(u"Car_Object", 7703),
			LDF_COLLECTION_INIT_ENTRY(u"Race_PathName", "MainPath"),
			LDF_COLLECTION_INIT_ENTRY(u"Current_Lap", 1),
			LDF_COLLECTION_INIT_ENTRY(u"Number_of_Laps", 3), // Number of Laps to complete the Race
			LDF_COLLECTION_INIT_ENTRY(u"activityID", 39),

			LDF_COLLECTION_INIT_ENTRY(u"Place_1", 100),
			LDF_COLLECTION_INIT_ENTRY(u"Place_2", 90),
			LDF_COLLECTION_INIT_ENTRY(u"Place_3", 80),
			LDF_COLLECTION_INIT_ENTRY(u"Place_4", 70),
			LDF_COLLECTION_INIT_ENTRY(u"Place_5", 60),
			LDF_COLLECTION_INIT_ENTRY(u"Place_6", 50),

			// -- Reward % Rating --
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_1", 15),
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_2", 25),
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_3", 50),
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_4", 85),
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_5", 90),
			LDF_COLLECTION_INIT_ENTRY(u"Num_of_Players_6", 100),

			LDF_COLLECTION_INIT_ENTRY(u"Number_of_Spawn_Groups", 1), // INT
			LDF_COLLECTION_INIT_ENTRY(u"Red_Spawners", 4847),
			LDF_COLLECTION_INIT_ENTRY(u"Blue_Spawners", 4848),
			LDF_COLLECTION_INIT_ENTRY(u"Blue_Flag", 4850),
			LDF_COLLECTION_INIT_ENTRY(u"Red_Flag", 4851),
			LDF_COLLECTION_INIT_ENTRY(u"Red_Point", 4846),
			LDF_COLLECTION_INIT_ENTRY(u"Blue_Point", 4845),
			LDF_COLLECTION_INIT_ENTRY(u"Red_Mark", 4844),
			LDF_COLLECTION_INIT_ENTRY(u"Blue_Mark", 4843)
		};
		
		print("ConfigureRacingControl...");
		// self:ConfigureRacingControl{parameters=racingParams}
		print("...Done");
	}

/*
function onSetActivityUserData(self, msg)

	local playerID = msg.userID:GetID()
	local existing = self:GetVar(playerID)

	if not existing then
		local takeCost = self:ChargeActivityCost{user = msg.userID}.bSucceeded
		self:SetVar(playerID, true)
	end
end

--------------------------------------------------------------------------------
*/
};
#endif 