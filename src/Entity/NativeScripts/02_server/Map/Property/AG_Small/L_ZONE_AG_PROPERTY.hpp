#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__PROPERTY__AG_SMALL__L_ZONE_AG_PROPERTY_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__PROPERTY__AG_SMALL__L_ZONE_AG_PROPERTY_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"
#include "Entity/NativeScripts/zone/PROPERTY/L_BASE_PROPERTY_SERVER.hpp"

class NATIVESCRIPT__02SERVER__MAP__PROPERTY__AG_SMALL__L_ZONE_AG_PROPERTY : public NATIVESCRIPT__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER {
	const bool OwnerChecked = false;
	const std::u16string GUIDMaelstrom = u"{7881e0a1-ef6d-420c-8040-f59994aa3357}"; // ambient sounds for when the Maelstrom is on
	const std::u16string GUIDPeaceful = u"{c5725665-58d0-465f-9e11-aeb1d21842ba}"; // happy ambient sounds when no maestrom is preset

	NSPropertyList Group = {
		{u"Guard", {u"Guard"}}, // mission giver npc
		{u"PropertyPlaque", {u"PropertyPlaque"}}, // make sure this matching the client script
		{u"PropertyVendor", {u"PropertyVendor"}}, // the object the player actually rents the property from
		{u"Land_Target", {u"Land_Target"}},
		{u"Spider_Scream", {u"Spider_Scream"}}, // mountain emitter for Boss screaming
		{u"ROF_Targets", {u"ROF_Targets_00", u"ROF_Targets_01", u"ROF_Targets_02", u"ROF_Targets_03", u"ROF_Targets_04"}}, // the groups of Spider Boss attack targets
		{u"SpiderEggs", {u"SpiderEggs"}}, // the smashable egg objects capable of hatching Dark Spiderlings
		{u"Rocks", {u"Rocks"}}, // the smashable rock objects
		{u"Enemies", {u"SpiderBoss"}}, // the Spider Boss AI instance
		// {u"ZoneVolumes", {u"Zone1Vol", u"Zone2Vol", u"Zone3Vol", u"Zone4Vol", u"Zone5Vol", u"Zone6Vol", u"Zone7Vol", u"Zone8Vol", u"AggroVol", u"TeleVol"}}, // each group contains a single volume that handles zone sensing for the map's four quadrants
		{u"FXManager", { u"FXObject" }} // the hidden object underground(small yellow box) that controls all the env fx for the map
	};

	NSPropertyList Spawners = {
		{u"Enemy" 			, {u"SpiderBoss"}}, // this can be as many spawner networks as necessary, but all spawner networks with enemies should be listed
		{u"BossSensors"		, {u"Zone1Vol", u"Zone2Vol", u"Zone3Vol", u"Zone4Vol", u"Zone5Vol", u"Zone6Vol", u"Zone7Vol", u"Zone8Vol", u"RFS_Targets", u"AggroVol", u"TeleVol"}},
		{u"Land_Target"		, {u"Land_Target"}},
		{u"Spider_Scream"	, {u"Spider_Scream"}}, // mountain emitter for Boss screaming
		{u"ROF_Targets" 	, {u"ROF_Targets_00", u"ROF_Targets_01", u"ROF_Targets_02", u"ROF_Targets_03", u"ROF_Targets_04"}}, // the spawner network for the Spider Boss attack targets
		{u"PropMG"  		, {u"PropertyGuard"}}, // spawns the mission giver for this property
		{u"FXManager"		, {u"FXObject"}}, // the hidden object underground (small yellow box) that controls all the env fx for the map
		{u"PropObjs"		, {u"BankObj"}}, // spawns objects that player's can interact with once a property is claimed
		{u"SpiderEggs"		, {u"SpiderEggs"}}, // the smashable egg objects capable of hatching Dark Spiderlings
		{u"Rocks"			, {u"Rocks"}}, // the smashable rock objects
		{u"SpiderRocket"	, { u"SpiderRocket_Bot",u"SpiderRocket_Mid",u"SpiderRocket_Top" }}, // spawns the pieces of the Spider Boss entry rocket
		{u"AmbientFX"		, { u"BirdFX",u"SunBeam" }}  // the ambient happy effects for the property, they are on by default and are turned off if maelstrom is spawned
	};

	// Special spawner networks that differentiate the Property versus the Instance
	NSPropertyList PropertySpawners = {
		{u"Mailbox", {u"Mailbox"}}, // property mailbox
		{u"Launcher", {u"Launcher"}} // property launcher
	};

	NSPropertyList InstanceSpawners = {
		{u"Instancer", {u"Instancer"}} // instance interact
	};

	// player flags. These have to be different for each property map. these are set up in the db
	NSPropertyList Flags = {
		{u"defeatedProperty", {u"71"}}, // when the player builds the claim marker, this flag is set
		{u"placedModel", {u"73"}}, // when a player places a model for the first time, this flag is set
		{u"guardFirstMission", {u"891"}}, // make sure the player has started the guards first mission, workaround for 1.9
		{u"guardMission", {u"320"}}, // last mission for the guard
		{u"password", {u"s3kratK1ttN"}}, // behavior password build qb object with behaviors
		{u"brickLinkMissionID", {u"951"}} // Achievement ID to complete on property rental
	};

	const std::u16string destroyedCinematic = u"DestroyMaelstrom";       // What to play when we clear the property

	void onPlayerLoaded(Entity::GameObject* self, GM::PlayerLoaded msg) {
		if (OwnerChecked == false)
			checkForOwner(self);
		DataTypes::LWOOBJID propertyOwner = self->GetVar<DataTypes::LWOOBJID>(u"PropertyOwner");
		bool rented = false;
		if (propertyOwner != std::uint64_t(0))
			rented = true;

		if (!rented) {
			// establish a player table(if one doesn't exist), and add the loaded 
			// player's ID to it
			std::list<DataTypes::LWOOBJID> playerTable = self->GetVarTList<DataTypes::LWOOBJID>(u"PlayerTable");
			DataTypes::LWOOBJID playerID = msg.playerID;
			playerTable.push_back(playerID);
			self->SetVar(u"PlayerTable", playerTable);

			// establish a player count (if one doesn't exist) and maintain accordingly
			std::int32_t numPlayers = self->GetVar<std::int32_t>(u"numberOfPlayers");
			numPlayers = numPlayers + 1;
			self->SetVar(u"numberOfPlayers", numPlayers);
		}

		basePlayerLoaded(self, msg);
	}

	// TODO: This is only temporary.
	void onStartup(Entity::GameObject* self) {
		setGameVariables(Group, Spawners, Flags);
	}

	// activates the spawner for the maelstrom spots
	void SpawnSpots(Entity::GameObject* self) {
		// activate all the ROF Targets
		for (auto v : Spawners.at(u"ROF_Targets")) {
			ActivateSpawner(self, v);
		}

		// activate the landing target
		ActivateSpawner(self, *Spawners.at(u"Land_Target").begin());
	}

	// deactive the spawner for the maelstrom spots and then delete them
	void KillSpots(Entity::GameObject* self) {
	
	}

	// activates the spawner for the Spider Boss crashed rocket
	void SpawnCrashedRocket(Entity::GameObject* self) {
		// activate all the rocket crash parts
		for (auto v : Spawners.at(u"SpiderRocket")) {
			ActivateSpawner(self, v);
		}
	}

	// deactives the spawners for the Spider Boss crashed rocket and
	// then delete them
	void KillCrashedRocket(Entity::GameObject* self) {
		// deactivate all the rocket crash parts
		for (auto v : Spawners.at(u"SpiderRocket")) {
			DeactivateSpawner(self, v);
			DestroySpawner(self, v);
		}
	}

	// turns all the maelstrom on if the player hasn't defeated the maelstrom for this property yet
	virtual void StartMaelstrom(Entity::GameObject* self, Entity::GameObject* player) {
		
		// activate the Spider Boss spawner and build all the necessary sensor objects
		ActivateSpawner(self, *Spawners.at(u"Enemy").begin());
		
		for (auto v : Spawners.at(u"BossSensors")) {
			ActivateSpawner(self, v);
		}

		if (Spawners.find(u"BehaviorObjs") != Spawners.end()) {
			for (auto v : Spawners.at(u"BehaviorObjs")) {
				ActivateSpawner(self, v);
			}
		}

		// activate the spawner for the fx
		ActivateSpawner(self, *Spawners.at(u"FXManager").begin());
		// activate the spawner for the mountain emitter
		ActivateSpawner(self, *Spawners.at(u"Spider_Scream").begin());
		// activate the spawner for the eggs
		ActivateSpawner(self, *Spawners.at(u"SpiderEggs").begin());
		// activate the spawner for the rocks
		ActivateSpawner(self, *Spawners.at(u"Rocks").begin());

		// activate the rocket crash parts
		SpawnCrashedRocket(self);

		if (Spawners.find(u"AmbientFX") != Spawners.end()) {
			for (auto v : Spawners.at(u"AmbientFX")) {
				DeactivateSpawner(self, v);
				DestroySpawner(self, v);
				ResetSpawner(self, v);
			}
		}

		startTornadoFX(self);

		// notify the client to change the env settings
		{GM::NotifyClientObject nmsg; nmsg.name = u"maelstromSkyOn"; GameMessages::Send(player, self->GetObjectID(), nmsg); }

	}
};
#endif 