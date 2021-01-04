#ifndef __ENTITY__NATIVESCRIPTS__AI__NS__L_NS_CONCERT_INSTRUMENT_QB_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__NS__L_NS_CONCERT_INSTRUMENT_QB_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"
#include "Entity/Components/SpawnerComponent.hpp"

class NATIVESCRIPT__AI__NS__L_NS_CONCERT_INSTRUMENT_QB : public NativeScript {

#pragma region CONSTANTS
private:
	// -- the object template for each instrument
	std::int32_t const CONST_INSTRUMENT_LOT_GUITAR = 4039;
	std::int32_t const CONST_INSTRUMENT_LOT_BASS = 4040;
	std::int32_t const CONST_INSTRUMENT_LOT_KEYBOARD = 4041;
	std::int32_t const CONST_INSTRUMENT_LOT_DRUM = 4042;

	// -- the anim used to show the player playing each instrument
	std::unordered_map<std::int32_t, std::u16string> const CONST_INSTRUMENT_ANIM = {
		{CONST_INSTRUMENT_LOT_GUITAR, u"guitar"},
		{CONST_INSTRUMENT_LOT_BASS, u"bass"},
		{CONST_INSTRUMENT_LOT_KEYBOARD, u"keyboard"},
		{CONST_INSTRUMENT_LOT_DRUM, u"drums"}
	};

	// -- the anim used to show the player smashing each instrument
	std::unordered_map<std::int32_t, std::u16string> const CONST_INSTRUMENT_SMASH_ANIM = {
		{CONST_INSTRUMENT_LOT_GUITAR, u"guitar-smash"},
		{CONST_INSTRUMENT_LOT_BASS, u"bass-smash"},
		{CONST_INSTRUMENT_LOT_KEYBOARD, u"keyboard-smash"},
		{CONST_INSTRUMENT_LOT_DRUM, u"keyboard-smash"} // -- has no smashing anim; use keys for now so that audio can attach smash sound
	};

	// -- the music used for each instrument
	std::unordered_map<std::int32_t, std::string> const CONST_INSTRUMENT_MUSIC = {
		{CONST_INSTRUMENT_LOT_GUITAR, "Concert_Guitar"},
		{CONST_INSTRUMENT_LOT_BASS, "Concert_Bass"},
		{CONST_INSTRUMENT_LOT_KEYBOARD, "Concert_Keys"},
		{CONST_INSTRUMENT_LOT_DRUM, "Concert_Drums"}
	};

	// -- path set up through Happy Flower for the cinematic for each instrument
	std::unordered_map<std::int32_t, std::u16string> const CONST_INSTRUMENT_CINEMATIC = {
		{CONST_INSTRUMENT_LOT_GUITAR, u"Concert_Cam_G"},
		{CONST_INSTRUMENT_LOT_BASS, u"Concert_Cam_B"},
		{CONST_INSTRUMENT_LOT_KEYBOARD, u"Concert_Cam_K"},
		{CONST_INSTRUMENT_LOT_DRUM, u"Concert_Cam_D"}
	};

	// -- the LOT for the left hand equippable item used for each instrument
	std::unordered_map<std::int32_t, std::int32_t> const CONST_INSTRUMENT_EQUIPPABLE_LOT_LEFT = {
		{CONST_INSTRUMENT_LOT_GUITAR, 4991},
		{CONST_INSTRUMENT_LOT_BASS, 4992},
		{CONST_INSTRUMENT_LOT_KEYBOARD, -1}, // -- no equippable used
		{CONST_INSTRUMENT_LOT_DRUM, 4995}
	};

	// -- the LOT for the right hand equippable item used for each instrument
	std::unordered_map<std::int32_t, std::int32_t> const CONST_INSTRUMENT_EQUIPPABLE_LOT_RIGHT = {
		{CONST_INSTRUMENT_LOT_GUITAR, -1},
		{CONST_INSTRUMENT_LOT_BASS, -1},
		{CONST_INSTRUMENT_LOT_KEYBOARD, -1},
		{CONST_INSTRUMENT_LOT_DRUM, 4996}
	};

	// -- whether or not to hide the completed quickbuild while the player is playing the instrument
	std::unordered_map<std::int32_t, bool> const CONST_INSTRUMENT_HIDE = {
		{CONST_INSTRUMENT_LOT_GUITAR, true},
		{CONST_INSTRUMENT_LOT_BASS, true},
		{CONST_INSTRUMENT_LOT_KEYBOARD, false},
		{CONST_INSTRUMENT_LOT_DRUM, false}
	};

	// -- once the smash anim starts playing, how long to wait before unequipping the instrument
	std::unordered_map<std::int32_t, std::float_t> const CONST_INSTRUMENT_UNEQUIP_TIME = {
		{CONST_INSTRUMENT_LOT_GUITAR, 1.033f},
		{CONST_INSTRUMENT_LOT_BASS, 0.75f},
		{CONST_INSTRUMENT_LOT_KEYBOARD, -1.f}, // -- has no equippables
		{CONST_INSTRUMENT_LOT_DRUM, 0.f} // -- has no smash anim, unequip immediately
	};

	// -- how much imagination is repeatedly drained while the instrument is being played
	std::int32_t const CONST_INSTRUMENT_IMAGINATION_COST = 2;

	// -- how often imagination is drained while playing the instrument
	std::float_t const CONST_INSTRUMENT_COST_FREQUENCY = 4.0f;


	std::int32_t const CONST_REBUILD_STATE_OPEN = 0;
	std::int32_t const CONST_REBUILD_STATE_COMPLETED = 2;
	std::int32_t const CONST_REBUILD_STATE_RESETTING = 4;
	std::float_t const CONST_UPDATE_FREQUENCY = 0.1f;
public:
#pragma endregion

	void onStartup(Entity::GameObject* self) {
		// -- needs to register with the server-side zone script
		// -- in case the player exits, logs out, etc. while playing an instrument.
		// -- the server-side zone script receives a PlayerExit msg, which it can then pass on to the instruments
		auto zControl = self->GetZoneInstance()->zoneControlObject;

		// self:SendLuaNotificationRequest{requestTarget = zControl, messageName = "PlayerExit"}
		// self:SendLuaNotificationRequest{ requestTarget = zControl, messageName = "PlayerResurrected" }
		
		// -- from client
		self->SetVar(u"bBeingPlayed", false);
		self->SetVar(u"activePlayer", DataTypes::LWOOBJID());

		// -- reset the object ID's of the equippable items
		self->SetVar(u"equippableLeft", DataTypes::LWOOBJID());
		self->SetVar(u"equippableRight", DataTypes::LWOOBJID());

		self->SetVar(u"checkingInstrumentAnim", false);
		self->SetVar(u"targetPositionX", 0.0f);
		self->SetVar(u"targetPositionZ", 0.0f);
		self->SetVar(u"leftInventoryObjectID", DataTypes::LWOOBJID());
		self->SetVar(u"rightInventoryObjectID", DataTypes::LWOOBJID());
		self->SetVar(u"reequipPlayer", DataTypes::LWOOBJID());

	}

	void onRebuildNotifyState(Entity::GameObject* self, GM::RebuildNotifyState msg) {
		auto playerID = msg.player;

		if (msg.iState == CONST_REBUILD_STATE_RESETTING || msg.iState == CONST_REBUILD_STATE_OPEN) {
			// -- clear the active player if the quickbuild state is being reset or not even started yet
			self->SetVar(u"activePlayer", DataTypes::LWOOBJID());
			// --print("Clearing active player, state="..msg.iState)
		}
		else {
			// -- set the active player in all other states
			self->SetVar(u"activePlayer", playerID);
			// self:SendLuaNotificationRequest{requestTarget = msg.player, messageName = "Die"}
			// --print("Setting active player: "..playerID..", state="..msg.iState)
		}
		
		if (msg.iState == CONST_REBUILD_STATE_COMPLETED) {
			storeObjectByName(self, u"activePlayer", self->GetZoneInstance()->objectsManager->GetObjectByID(msg.player));
			QuickBuildWasBuilt(self);
		}
	}

	void onFireEventServerSide(Entity::GameObject* self, GM::FireEventServerSide msg) {
		if (msg.args == u"stopPlaying") {
			StopPlayingInstrument(self);
		}
	}

	void AlterMusic(Entity::GameObject* self, std::string szMusicName, bool bActivate) {
		auto soundRepeaters = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"Audio-Concert", std::uint64_t(0), true);

		for (auto soundRepeater : soundRepeaters) {
			if (bActivate) {
				{GM::ActivateNDAudioMusicCue nmsg; nmsg.m_NDAudioMusicCueName = szMusicName; soundRepeater->CallMessage(nmsg); }
			}
			else {
				{GM::DeactivateNDAudioMusicCue nmsg; nmsg.m_NDAudioMusicCueName = szMusicName; soundRepeater->CallMessage(nmsg); }
			}
		}
	}

	void stopPlayerInteraction(Entity::GameObject* self, Entity::GameObject* player) {
		/* TODO */
	}

	void notifyPlayerExit(Entity::GameObject* self, Entity::GameObject* other, void* msg) {
		/* TODO */
	}

	void notifyDie(Entity::GameObject* self, Entity::GameObject* other, GM::Die msg) {
		/* TODO */
	}

	void notifyPlayerResurrected(Entity::GameObject* self, Entity::GameObject* other, void* msg) {
		/* TODO */
	}

	//  --------------------------------------------------------------
	//  -- a player left the zone
	//  	-- they could have exited via the x, the menu, or slash command
	//  	-- or logged out via the menu or slash command
	//  	-- or gone back to Character Select via the menu
	//  	-- or had a client crash
	//  	-- or the map could have been reloaded
	//  -- check if the player that left was the one playing this instrument
	//  	-- if so, stop the music
	//  --------------------------------------------------------------
	void CancelMusicIfPlayerLeft(Entity::GameObject* self, Entity::GameObject* exitPlayer) {
		/* TODO */
	}

	//  --------------------------------------------------------------
	//  -- a player left the zone
	//  	-- they could have exited via the x, the menu, or slash command
	//  	-- or logged out via the menu or slash command
	//  	-- or gone back to Character Select via the menu
	//  	-- or had a client crash
	//  	-- or the map could have been reloaded
	//  --------------------------------------------------------------
	void StopPlayingIfPlayerLeft(Entity::GameObject* self, Entity::GameObject* exitPlayer) {
		/* TODO */
	}

	void DecreasePlayersImagination(Entity::GameObject* self) {
		/* TODO */
	}

	bool IsPlayerOutOfImagination(Entity::GameObject* self) {
		return GetPlayersImagination(self) <= 0;
	}

	std::int32_t GetPlayersImagination(Entity::GameObject* self) {
		auto player = GetActivePlayer(self);
		if (player == nullptr)
			return 0;
		else
			return player->GetImagination();
	}

	void StartUpdateTimer(Entity::GameObject* self) {
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(1000 * CONST_UPDATE_FREQUENCY, u"CheckOnPlayer", self);
	}

	void StartImaginationCostTimer(Entity::GameObject* self) {
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(1000 * CONST_INSTRUMENT_COST_FREQUENCY, u"DecreaseImagination", self);
	}

	void onTimerDone(Entity::GameObject* self, GM::TimerDone msg) {
		if (msg.name == u"Hide") {
			RepositionPlayer(self, self->GetLOT());

			if (CONST_INSTRUMENT_HIDE.at(self->GetLOT())) {
				self->SetNetworkedVar(u"Hide", true);
			}
		}
		else if (msg.name == u"Play") {
			PlayInstrument(self);
		}
		
		/* TODO */
	}

	void StopPlayingInstrument(Entity::GameObject* self) {
		/* TODO */
	}

	void PlayInstrument(Entity::GameObject* self) {
		auto player = GetActivePlayer(self);

		if (player == nullptr) return;
		
		auto instrumentLOT = self->GetLOT();

		// -- remember that someone is using this instrument
		self->SetVar(u"bBeingPlayed", true);

		// -- load any related equippables (guitar, bass, drumsticks), which will equip onto the player
		LoadEquippables(self);

		// -- start keeping track of movement key presses long enough to know if the player wants to break out of playing the instument
		{GM::NotifyClientObject nmsg; nmsg.name = u"startPlaying"; nmsg.paramObj = player->GetObjectID(); GameMessages::Send(player, self->GetObjectID(), nmsg); }

		// -- show the cinematic
		PlayCinematic(self, instrumentLOT);

		// -- play the animation of the mini-fig playing the given instrument
		PlayInstrumentAnim(self, instrumentLOT);

		// -- start the appropriate music
		AffectMusic(self, instrumentLOT, true);

		// -- start a timer for when to check whether the instrument is still being played
		StartUpdateTimer(self);
	
		// -- start a timer for decreasing the player's imagination
		StartImaginationCostTimer(self);

		self->GetZoneInstance()->timer.AddTimerWithCancelMs(1000 * 20, u"AchievementTimer_20", self);
	}

	void AffectMusic(Entity::GameObject* self, std::int32_t instrumentLOT, bool bActivate) {
		auto szMusicName = CONST_INSTRUMENT_MUSIC.at(instrumentLOT);
		auto player = GetActivePlayer(self);

		if (player != nullptr) {
			AlterMusic(self, szMusicName, bActivate);

			if (bActivate) {
				storeObjectByName(self, u"musicPlayer", player);
			}
			else {
				self->SetVar(u"musicPlayer", false);
			}
		}
	}

	void PlayInstrumentAnim(Entity::GameObject* self, std::int32_t instrumentLOT) {
		auto player = GetActivePlayer(self);

		if (player != nullptr) {
			{GM::PlayAnimation nmsg; nmsg.animationID = CONST_INSTRUMENT_ANIM.at(instrumentLOT); nmsg.fPriority = 2.0f; GameMessages::Broadcast(player, nmsg); }
		}
	}

	void PlayCinematic(Entity::GameObject* self, std::int32_t instrumentLOT) {
		auto player = GetActivePlayer(self);

		if (player != nullptr) {
			auto szPathName = CONST_INSTRUMENT_CINEMATIC.at(instrumentLOT);

			{GM::PlayCinematic nmsg; nmsg.pathName = szPathName; GameMessages::Send(player, player->GetObjectID(), nmsg); }
		}
	}

	void QuickBuildWasBuilt(Entity::GameObject* self) {
		// -- set timers for when to hide the completed quickbuild and show the player playing the instrument
		// -- do this quicky so we don't have to watch the finished quickbuild model to slam into place
		// -- but we have to wait slightly first or get overridden by the quickbuild code
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(200, u"Hide", self);
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(100, u"Play", self);
	}

	void UnquipEquippables(Entity::GameObject* self, bool bRespawning, Entity::GameObject* respawnPlayer) {
		/* TODO */
	}

	void EndActivePlayer(Entity::GameObject* self) {
		/* TODO */
	}

	void RepositionPlayer(Entity::GameObject* self, std::int32_t instrumentLOT) {
		/* TODO */
	}

	void LoadEquippables(Entity::GameObject* self) {
		/* TODO */
	}

	void ReequipInventoryItems(Entity::GameObject* self, bool bRespawning, std::list<DataTypes::LWOOBJID>& respawnPlayerTable) {
		/* TODO */
	}

	Entity::GameObject* GetActivePlayer(Entity::GameObject* self) {
		return getObjectByName(self, u"activePlayer");
	}

	DataTypes::LWOOBJID GetActivePlayerID(Entity::GameObject * self) {
		auto player = GetActivePlayer(self);
		if (player != nullptr)
			return player->GetObjectID();
		return DataTypes::LWOOBJID();
	}

	void storeObjectByName(Entity::GameObject* self, std::u16string varName, Entity::GameObject* object) {
		if (object == nullptr || varName == u"") return;

		self->SetVar(varName, object->GetObjectID());
	}

	Entity::GameObject* getObjectByName(Entity::GameObject* self, std::u16string varName) {
		if (varName == u"") return nullptr;

		auto targetID = self->GetVar(varName);
		if (targetID.operator bool()) {
			return self->GetZoneInstance()->objectsManager->GetObjectByID(targetID.operator DataTypes::LWOOBJID());
		}
		return nullptr;
	}
};
#endif 