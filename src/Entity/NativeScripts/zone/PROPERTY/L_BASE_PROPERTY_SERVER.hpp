#ifndef __ENTITY__NATIVESCRIPTS__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"
#include "Entity/Components/SpawnerComponent.hpp"

class NATIVESCRIPT__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER : public NativeScript {

public:

	const bool OwnerChecked = false;
	const std::u16string GUIDMaelstrom = u"{7881e0a1-ef6d-420c-8040-f59994aa3357}"; // ambient sounds for when the Maelstrom is on
	const std::u16string GUIDPeaceful = u"{c5725665-58d0-465f-9e11-aeb1d21842ba}"; // happy ambient sounds when no maestrom is preset

	// Define empty tables that will be set from the level specific script
	typedef std::unordered_map<std::u16string, std::list<std::u16string>> NSPropertyList;
	NSPropertyList Group = {};
	NSPropertyList Spawners = {};
	NSPropertyList Flags = {};

	// variables passed of the level specific script that are used throughout the base script
	virtual void setGameVariables(NSPropertyList passedGroups, NSPropertyList passedSpawners, NSPropertyList passedFlags) {
		if (passedGroups.begin() != passedGroups.end())
			Group = passedGroups;
		if (passedSpawners.begin() != passedSpawners.end())
			Spawners = passedSpawners;
		if (passedFlags.begin() != passedFlags.end())
			Flags = passedFlags;
	}

	virtual void checkForOwner(Entity::GameObject* self) {

	}
	
	virtual void onFireEvent(Entity::GameObject* self, GM::FireEventServerSide msg) {

	}

	virtual void basePlayerLoaded(Entity::GameObject* self, GM::PlayerLoaded msg) {
		if (OwnerChecked == false)
			checkForOwner(self);
		DataTypes::LWOOBJID propertyOwner = self->GetVar<DataTypes::LWOOBJID>(u"PropertyOwner");
		bool rented = false;
		if (propertyOwner != 0ULL)
			rented = true;

		Entity::GameObject* player = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.playerID);
		#ifdef DEBUG
		self->SetVar(u"IsInternal", true);
		#else
		self->SetVar(u"IsInternal", false);
		#endif

		// if the property is rented, delete all the objects that may be on the property
		if (rented) {
			// kill the tornado effect object
			if (!self->GetVar(u"FXObjectGone")) {
				self->GetZoneInstance()->timer.AddTimerWithCancelMs(1000, u"killFXObject", self);
			}
			// TODO: player:Play2DAmbientSound{m_NDAudioEventGUID = GUIDPeaceful}

			/*-- spawn the property safe object
			ActivateSpawner(self,Spawners.PropObjs)
		
			-- tell the client script that the property is rented and who the renter is
			self:SetNetworkVar("renter",propertyOwner)

			--  check to see if the player standing on the property is the person who owns the property
			if player:GetID() ~= propertyOwner then
				return
			end*/
		}
		// if the property hasnt been rented, then assume that this is the players property to rent (if this is broken, it is through code)
		else {
			// flag for if the player has defeated the maelstrom property yet
			bool defeatedflag = player->GetFlag(std::stoi(StringUtils::to_string(*Flags.at(u"defeatedProperty").begin())));
			// tell the client script that the property is not rented
			self->SetNetworkedVar(u"unclaimed", true);
			
			self->SetVar(u"playerID", StringUtils::to_u16string(std::to_string(player->GetObjectID())));

			// check to see if the player has defeated the maelstrom before (they could defeat it and leave without renting it)
			if (!defeatedflag) {
				// custom function that starts all teh maelstrom 
				StartMaelstrom(self, player);
				SpawnSpots(self);
				self->SetVar(u"playerID", StringUtils::to_u16string(std::to_string(player->GetObjectID())));
				//player:Play2DAmbientSound{m_NDAudioEventGUID = GUIDMaelstrom}
			}

			else {
			/*	player:Play2DAmbientSound{ m_NDAudioEventGUID = GUIDPeaceful }
				GAMEOBJ:GetTimer() : AddTimerWithCancel(1, "killFXObject", self)*/
			}
		}

		// spawn the Property Guard?
		propGuardCheck(self, msg);
	}

	virtual void propGuardCheck(Entity::GameObject* self, GM::PlayerLoaded msg) {

	}

	virtual void baseZonePropertyRented(Entity::GameObject* self, void* msg) {

	}

	virtual void baseZonePropertyModelPlaced(Entity::GameObject* self, void* msg) {

	}

	virtual void KillClouds(Entity::GameObject* self) {

	}

	// activates the spawner for the maelstrom spots
	virtual void SpawnSpots(Entity::GameObject* self) {
		ActivateSpawner(self, *Spawners.at(u"FXSpots").begin());
	}

	virtual void KillSpots(Entity::GameObject* self) {

	}

	// turns all the maelstrom on if the player hasn't defeated the maelstrom for this property yet
	virtual void StartMaelstrom(Entity::GameObject* self, Entity::GameObject* player) {
		// activate all the enemy spawners
		for (auto v : Spawners.at(u"Enemy")) {
			ActivateSpawner(self, v);
		}

		for (auto v : Spawners.at(u"BehaviorObjs")) {
			ActivateSpawner(self, v);
		}

		// activate the damaging clouds spawner
		ActivateSpawner(self, *Spawners.at(u"DamageFX").begin());

		// activate the spawner for the generator
		ActivateSpawner(self, *Spawners.at(u"Generator").begin());
		ActivateSpawner(self, *Spawners.at(u"GeneratorFX").begin());
		ActivateSpawner(self, *Spawners.at(u"FXManager").begin());
		ActivateSpawner(self, *Spawners.at(u"ImagOrb").begin());
		ActivateSpawner(self, *Spawners.at(u"Smashables").begin());

		// make sure the spawner for the claimmarker is off
		DeactivateSpawner(self, *Spawners.at(u"ClaimMarker").begin());
		ResetSpawner(self, *Spawners.at(u"ClaimMarker").begin());

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
		// have the generator tell the script when it dies
		// sendNotification(self,Group.Generator,"Die","startGenerator")
		// sendNotification(self, Group.ImagOrb, "CollisionPhantom", "startOrb")
	}

	virtual void startTornadoFX(Entity::GameObject* self) {

	}

	virtual void baseNotifyDie(Entity::GameObject* self, Entity::GameObject* other, void * msg) {

	}

	virtual void baseNotifyCollisionPhantom(Entity::GameObject* self, void* other, void* msg) {
	
	}

	virtual void baseNotifyRebuildComplete(Entity::GameObject* self, Entity::GameObject* other, void* msg) {

	}

	virtual void basePlayerExit(Entity::GameObject* self, Entity::GameObject* other) {

	}

	virtual void sendNotification(Entity::GameObject* self, Entity::GameObject* obj, std::u16string msgName, TimerDone timer) {

	}

	virtual void KillObject(Entity::GameObject* self) {

	}

	virtual void ObjectRequestDie(Entity::GameObject* self, std::list<Entity::GameObject*> objTable) {

	}

	virtual void DestroySpawner(Entity::GameObject* self, std::u16string spawnerName) {

	}

	// activates spawners
	virtual void ActivateSpawner(Entity::GameObject* self, std::u16string spawnerName) {
		SpawnerComponent* spawnerID = self->GetZoneInstance()->objectsManager->GetSpawnerByName(spawnerName);
		if (spawnerID != nullptr)
			spawnerID->SpawnerActivate();
		else
			WarningPrint(self, "WARNING: No Spawner found for spawner named " + StringUtils::to_string(spawnerName));
	}

	virtual void DeactivateSpawner(Entity::GameObject* self, std::u16string spawnerName) {

	}

	virtual void ResetSpawner(Entity::GameObject* self, std::u16string spawnerName) {

	}

	virtual void WarningPrint(Entity::GameObject* self, std::string msg) {
		if (self->GetVar(u"IsInternal")) {
			print(msg);
		}
	}

	virtual void baseTimerDone(Entity::GameObject* self, TimerDone msg) {

	}

};
#endif 