#ifndef __ENTITY__NATIVESCRIPTS__ZONE__AG__L_ZONE_AG_SURVIVAL_HPP__
#define __ENTITY__NATIVESCRIPTS__ZONE__AG__L_ZONE_AG_SURVIVAL_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

#include "Entity/NativeScripts/ai/MINIGAME/SURVIVAL/BASE_SURVIVAL_SERVER.hpp"

class NATIVESCRIPT__ZONE__AG__L_ZONE_AG_SURVIVAL : public NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER {
	
public:
	friend class NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER;
	//--------------------------------------------------------------
	//-- AG Survival Instance Server Zone Script: Including this 
	//-- file lets you set the custom variables for the Survival game.
	//-- updated mrb... 1/7/10
	//-- updated abeechler ... 4/19/11 - refactored mission update format and checks
	//--------------------------------------------------------------
	//
	//--------------------------------------------------------------
	//-- Includes
	//--------------------------------------------------------------
	//require('ai/MINIGAME/Survival/BASE_SURVIVAL_SERVER')
	//


	NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_gConstants gConstants;
	NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tMobSets tMobSets;
	NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks tSpawnerNetworks;
	std::map<std::int32_t, std::int32_t> missionsToUpate;

	//--//////////////////////////////////////////////////////////////////////////////////
	//-- User Config local variables
	//local gConstants = 
	//{
	//    acceptedDelay = 60,         -- how long to wait after one person has presed start to start the match
	//    startDelay = 2,             -- how long to wait after all the players have accepted before starting the game.
	//    waveTime = 7,               -- how often to spawn a new wave of mobs
	//    rewardInterval = 5,         -- how many waves to wait to drop a reward and give the player a gConstants.coolDownTime
	//    coolDownTime = 10,          -- how long to wait between waves of gConstants.rewardInterval
	//    startMobSet2 = 5,           -- wave number to start spawning set 2
	//    startMobSet3 = 15,          -- wave number to start spawning set 3
	//    unlockNetwork3 = 10,
	//    bUseMobLots = true,
	//    iLotPhase = 1,
	//    
	//    baseMobsStartTierAt = { 8, 13, 18, 23, 28, 32, },   -- wave number to start spawning tier mobs    
	//    randMobsStartTierAt = { 2, 10, 15, 20, 25, 30, },   -- wave number to start spawning tier mobs
	//
	//    returnZone = 1100,          -- map number the player will return to on exit
	//    returnLoc = { x = 125, y = 376, z = -175} -- {x,y,z} location that the player will be teleported to in the returnZone on exit
	//}

	NATIVESCRIPT__ZONE__AG__L_ZONE_AG_SURVIVAL() : NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER() {
		{

			gConstants.acceptedDelay = 60;			//-- how long to wait after one person has presed start to start the match
			gConstants.startDelay = 2;             //-- how long to wait after all the players have accepted before starting the game.
			gConstants.waveTime = 7;               //-- how often to spawn a new wave of mobs
			gConstants.rewardInterval = 5;         //-- how many waves to wait to drop a reward and give the player a gConstants.coolDownTime
			gConstants.coolDownTime = 10;          //-- how long to wait between waves of gConstants.rewardInterval
			gConstants.startMobSet2 = 5;           //-- wave number to start spawning set 2
			gConstants.startMobSet3 = 15;          //-- wave number to start spawning set 3
			gConstants.unlockNetwork3 = 10;
			gConstants.bUseMobLots = true;
			gConstants.iLotPhase = 1;

			gConstants.baseMobsStartTierAt = { 8, 13, 18, 23, 28, 32, };   //-- wave number to start spawning tier mobs    
			gConstants.randMobsStartTierAt = { 2, 10, 15, 20, 25, 30, };   //-- wave number to start spawning tier mobs

			gConstants.returnZone = 1100;          //-- map number the player will return to on exit
			gConstants.returnLoc = { 125, 376, -175 }; //-- {x,y,z} location that the player will be teleported to in the returnZone on exit
		}

	//--============================================================
	//
	//-- Mob Sets
	//local tMobSets = 
	//{
	//    mobLots = 
	//    {
	//        MobA = {6351, 8088, 8089},
	//        MobB = {6668, 8090, 8091},
	//        MobC = {6454, 8096, 8097},
	//    },
	//    -- these will always spawn *************************************************************************
	//    -- ** format ** tier# = {{Mob1, Mob2, Mob3},}, 
	//    baseMobSet = -- tMobSets.baseMobSet
	//    {
	//            tier1 = { {3, 0, 0}, },
	//            tier2 = { {2, 1, 0}, },
	//            tier3 = { {4, 1, 0}, },
	//            tier4 = { {1, 2, 0}, },
	//            tier5 = { {0, 1, 1}, },
	//            tier6 = { {0, 2, 2}, },
	//    },    
	//        
	//    -- randomly pick from these sets to spawn along with the base set **********************************
	//    -- ** format ** tier# = {{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3}}, 
	//    randMobSet = -- tMobSets.randMobSet
	//    {
	//            tier1 = { {4, 0, 0},{4, 0, 0},{4, 0, 0},{4, 0, 0},{3, 1, 0} },
	//            tier2 = { {4, 1, 0},{4, 1, 0},{4, 1, 0},{4, 1, 0},{2, 1, 1} },
	//            tier3 = { {1, 2, 0},{1, 2, 0},{1, 2, 0},{1, 2, 0},{0, 1, 1} },
	//            tier4 = { {1, 2, 1},{1, 2, 1},{1, 2, 1},{0, 2, 1},{0, 2, 2} },
	//            tier5 = { {0, 1, 2},{0, 1, 2},{0, 1, 2},{0, 1, 3},{0, 1, 3} },
	//            tier6 = { {0, 2, 3},{0, 2, 3},{0, 2, 3},{0, 2, 3},{0, 2, 3} },
	//    },
	//}

		{

			tMobSets.mobLots = {
				 {"MobA", {6351, 8088, 8089}},
				 {"MobB", {6668, 8090, 8091}},
				 {"MobC", {6454, 8096, 8097}},
			};
			// -- these will always spawn *************************************************************************
			// -- ** format ** tier# = {{Mob1, Mob2, Mob3},}, 
			tMobSets.baseMobSet = {
				{"tier1", { {3, 0, 0}, }},
	            {"tier2", { {2, 1, 0}, }},
	            {"tier3", { {4, 1, 0}, }},
	            {"tier4", { {1, 2, 0}, }},
	            {"tier5", { {0, 1, 1}, }},
	            {"tier6", { {0, 2, 2}, }},
			};

			// -- randomly pick from these sets to spawn along with the base set **********************************
			// -- ** format ** tier# = {{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3},{Mob1, Mob2, Mob3}}, 
			tMobSets.randMobSet = {
	            {"tier1", { {4, 0, 0},{4, 0, 0},{4, 0, 0},{4, 0, 0},{3, 1, 0} }},
	            {"tier2", { {4, 1, 0},{4, 1, 0},{4, 1, 0},{4, 1, 0},{2, 1, 1} }},
	            {"tier3", { {1, 2, 0},{1, 2, 0},{1, 2, 0},{1, 2, 0},{0, 1, 1} }},
	            {"tier4", { {1, 2, 1},{1, 2, 1},{1, 2, 1},{0, 2, 1},{0, 2, 2} }},
	            {"tier5", { {0, 1, 2},{0, 1, 2},{0, 1, 2},{0, 1, 3},{0, 1, 3} }},
	            {"tier6", { {0, 2, 3},{0, 2, 3},{0, 2, 3},{0, 2, 3},{0, 2, 3} }},
			};
		}
	//--============================================================
	//
	//-- Spawn Networks 
	//local tSpawnerNetworks = 
	//{
	//    baseNetworks = -- tSpawnerNetworks.baseNetworks
	//    {   set = 'baseMobSet',
	//        {
	//            spawnerName = {'Base_MobA', 'Base_MobB','Base_MobC'}, -- name of base spawner network without from HF
	//            spawnerNum = '', 
	//            bIsLocked = false,
	//            bIsActive = false,
	//        },
	//    },
	//    --***************************************
	//
	//    randNetworks = -- tSpawnerNetworks.randNetworks
	//    {   set = 'randMobSet',
	//        --********************************** Spawner 1
	//        {   
	//            spawnerName = {'MobA_','MobB_','MobC_'},        -- name of the MobA spawner networks without the #'s from HF
	//            spawnerNum = '01',                              -- number of MobB spawner networks  
	//            bIsLocked = false,
	//            bIsActive = false,
	//        },                    
	//        --********************************** Spawner 2
	//        {   
	//            spawnerName = {'MobA_','MobB_','MobC_'},        -- name of the MobA spawner networks without the #'s from HF
	//            spawnerNum = '02',                              -- number of MobB spawner networks  
	//            bIsLocked = false,
	//            bIsActive = false,
	//        },       
	//        --********************************** Spawner 3
	//        {   
	//            spawnerName = {'MobA_','MobB_','MobC_'},        -- name of the MobA spawner networks without the #'s from HF
	//            spawnerNum = '03',                              -- number of MobB spawner networks  
	//            bIsLocked = true,
	//            bIsActive = false,
	//        },
	//    },
	//    --***************************************
	//  
	//    -- Reward Networks
	//    rewardNetworks = -- tSpawnerNetworks.rewardNetworks
	//    {
	//        {
	//            --******************************** Rewards 1
	//            spawnerName = {'Rewards_'},             -- name of the reward 1 spawner network without the #'s from HF
	//            spawnerNum = '01',    
	//            bIsLocked = false,
	//        },
	//    },
	//    
	//    -- Smashable Networks
	//    smashNetworks = -- tSpawnerNetworks.smashNetworks
	//    {
	//        {
	//            --******************************** Smashables 1
	//            spawnerName = {'Smash_'},               -- name of the reward 1 spawner network without the #'s from HF
	//            spawnerNum = '01',    
	//            bIsLocked = false,
	//        },
	//    } ,
	//}
	//

		{
			{
				tSpawnerNetworks.baseNetworks.set = u"baseMobSet";
				{
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"Base_MobA", u"Base_MobB", u"Base_MobC" };
					tempNetwork.spawnerNum = u"";
					tempNetwork.bIsLocked = false;
					tempNetwork.bIsActive = false;
					tSpawnerNetworks.baseNetworks.networks.push_back(tempNetwork);
				}
			}

			{
				tSpawnerNetworks.randNetworks.set = u"randMobSet";
				// --********************************** Spawner 1
				{
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"MobA_", u"MobB_", u"MobC_" };			//--name of the MobA spawner networks without the #'s from HF
					tempNetwork.spawnerNum = u"01";										//--number of MobB spawner networks
					tempNetwork.bIsLocked = false;
					tempNetwork.bIsActive = false;
					tSpawnerNetworks.randNetworks.networks.push_back(tempNetwork);
				}
				// --********************************** Spawner 2
				{
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"MobA_", u"MobB_", u"MobC_" };			//--name of the MobA spawner networks without the #'s from HF
					tempNetwork.spawnerNum = u"02";										//--number of MobB spawner networks
					tempNetwork.bIsLocked = false;
					tempNetwork.bIsActive = false;
					tSpawnerNetworks.randNetworks.networks.push_back(tempNetwork);
				}
				// --********************************** Spawner 3
				{
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"MobA_", u"MobB_", u"MobC_" };			//--name of the MobA spawner networks without the #'s from HF
					tempNetwork.spawnerNum = u"03";										//--number of MobB spawner networks
					tempNetwork.bIsLocked = true;
					tempNetwork.bIsActive = false;
					tSpawnerNetworks.randNetworks.networks.push_back(tempNetwork);
				}
			}

			{
				{
					//--******************************** Rewards 1
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"Rewards_" };			//-- name of the reward 1 spawner network without the #'s from HF
					tempNetwork.spawnerNum = u"01";
					tempNetwork.bIsLocked = false;
					tSpawnerNetworks.rewardNetworks.networks.push_back(tempNetwork);
				}
			}

			{
				{
					//--******************************** Smashables 1
					NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER::NATIVESCRIPT__AI__MINIGAME__SURVIVAL__BASE_SURVIVAL_SERVER_tSpawnerNetworks_SpawnerNetworkSet_SpawnerNetwork tempNetwork;
					tempNetwork.spawnerName = { u"Smash_" };			//-- name of the reward 1 spawner network without the #'s from HF
					tempNetwork.spawnerNum = u"01";
					tempNetwork.bIsLocked = false;
					tSpawnerNetworks.smashNetworks.networks.push_back(tempNetwork);
				}
			}
				
		}

	//-- Survival associated mission target times, keyed by missionID, in seconds
	//local missionsToUpdate = {	[479] = 60,
	//							[1153] = 180,
	//							[1618] = 420,
	//							[1648] = 420,
	//							[1628] = 420,
	//							[1638] = 420,
	//							[1412] = 120,
	//							[1510] = 120,
	//							[1547] = 120,
	//							[1584] = 120,
	//							[1426] = 300,
	//							[1524] = 300,
	//							[1561] = 300,
	//							[1598] = 300,
	//							[1865] = 180}
	//

		missionsToUpate = {
			{479, 60},
			{1153, 180},
			{1618, 420},
			{1648, 420},
			{1628, 420},
			{1638, 420},
			{1412, 120},
			{1510, 120},
			{1547, 120},
			{1584, 120},
			{1426, 300},
			{1524, 300},
			{1561, 300},
			{1598, 300},
			{1865, 180}
		};
	}

	//--============================================================
	//-- Game messages sent to the BASE_SURVIVAL_SERVER.lua file, these
	//-- must be in this script. Only change to add custom functionality, 
	//-- but leav e the base*message*(self, msg, newMsg) in the function.
	//--============================================================
	//
	//----------------------------------------------------------------
	//-- Received when the script is loaded
	//----------------------------------------------------------------
	//function onStartup(self)
	//    -- send the configured variables to the base script
	//    setGameVariables(gConstants, tMobSets, tSpawnerNetworks, missionsToUpdate)
	//    baseStartup(self, newMsg)
	//end
	void onStartup(Entity::GameObject* self) {
		self->SetNetworkedVar(u"NumberOfPlayers", (std::int64_t)1);
		setGameVariables(gConstants, tMobSets, tSpawnerNetworks, missionsToUpate);
		baseStartup(self, nullptr);
	}
	//
	//----------------------------------------------------------------
	//-- Player is fully loaded and has completed the load handshake process.
	//----------------------------------------------------------------
	//function onPlayerReady(self, msg)
	//    basePlayerReady(self, msg, newMsg)
	//end
	void onPlayerReady(Entity::GameObject* self, GM::PlayerReady msg) {
		basePlayerReady(self, msg, nullptr);
	}
	//
	//----------------------------------------------------------------
	//-- Player has loaded into the map
	//----------------------------------------------------------------
	//function onPlayerLoaded(self, msg)
	//    basePlayerLoaded(self, msg, newMsg)
	//end
	void onPlayerLoaded(Entity::GameObject* self, GM::PlayerLoaded msg) {
		basePlayerLoaded(self, msg, nullptr);


		// TODO: Remove in future:
		MissionManager::LaunchTaskEvent(Enums::EMissionTask::SCRIPT, self, msg.playerID, 1, 479);
	}
	//
	//----------------------------------------------------------------
	//-- Player has exited the map
	//----------------------------------------------------------------
	//function onPlayerExit(self, msg)
	//    basePlayerExit(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- Received a fire event messaged from the client
	//----------------------------------------------------------------
	//function onFireEventServerSide(self, msg)   
	//    baseFireEventServerSide(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- Received a fire event messaged from someplace on the server
	//----------------------------------------------------------------
	//function onFireEvent(self,msg)   
	//    baseFireEvent(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- A player had died
	//----------------------------------------------------------------
	//function onPlayerDied(self, msg)
	//    basePlayerDied(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- A player has respawned
	//----------------------------------------------------------------
	//function onPlayerResurrected(self, msg)
	//    basePlayerResurrected(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- Received a notify object message 
	//----------------------------------------------------------------
	//function onNotifyObject(self, msg)
	//    baseNotifyObject(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- This is called when players hit the UI to exit or stop the game.
	//----------------------------------------------------------------
	//function onMessageBoxRespond(self,msg)
	//    baseMessageBoxRespond(self, msg, newMsg)
	//end
	//
	//----------------------------------------------------------------
	//-- Notification that a ui element used.
	//----------------------------------------------------------------
	//function onActivityStateChangeRequest(self,msg)
	//    baseActivityStateChangeRequest(self, msg, newMsg)
	//end

};

#endif 