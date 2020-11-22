#ifndef __ENTITY__NATIVESCRIPTS__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__ZONE__PROPERTY__L_BASE_PROPERTY_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Missions/MissionManager.hpp"

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
	void setGameVariables(NSPropertyList passedGroups, NSPropertyList passedSpawners, NSPropertyList passedFlags) {
		if (passedGroups.begin() != passedGroups.end())
			Group = passedGroups;
		if (passedSpawners.begin() != passedSpawners.end())
			Spawners = passedSpawners;
		if (passedFlags.begin() != passedFlags.end())
			Flags = passedFlags;
	}

	void checkForOwner(Entity::GameObject* self) {

	}
	
	void onFireEvent(Entity::GameObject* self, GM::FireEventServerSide msg) {

	}

	void basePlayerLoaded(Entity::GameObject* self, GM::PlayerLoaded msg) {
		if (OwnerChecked == false)
			checkForOwner(self);
		DataTypes::LWOOBJID propertyOwner = self->GetVar<DataTypes::LWOOBJID>(u"PropertyOwner");
		bool rented = false;
		if (propertyOwner != 0ULL)
			rented = true;

		Enity::GameObject* player = self->GetZoneInstance()->objectsManager->GetObjectByID(msg.playerID);
		self->SetVar(u"IsInternal",
			#ifdef DEBUG true 
			#else false
		#endif
		);

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
			bool defeatedflag = player->GetFlag(std::stoi(StringUtils::to_string(*Flags.at(u"defeatedProperty")))).begin()));
			// tell the client script that the property is not rented
			self->SetNetworkedVar(u"unclaimed", true);
			
			self->SetVar(u"playerID", StringUtils::to_u16string(std::to_string(player->GetObjectID())));

			// check to see if the player has defeated the maelstrom before (they could defeat it and leave without renting it)
			if (!defeatedflag) {
				/*-- custom function that starts all teh maelstrom 
				StartMaelstrom(self,player)
				SpawnSpots(self)
				self:SetVar("playerID",player:GetID())
				player:Play2DAmbientSound{m_NDAudioEventGUID = GUIDMaelstrom}*/
			}

			else {
			/*	player:Play2DAmbientSound{ m_NDAudioEventGUID = GUIDPeaceful }
				GAMEOBJ:GetTimer() : AddTimerWithCancel(1, "killFXObject", self)*/
			}
		}

		// spawn the Property Guard?
		propGuardCheck(self, msg);
	}

	void propGuardCheck(Entity::GameObject* self, GM::PlayerLoaded msg) {

	}

	void baseZonePropertyRented(Entity::GameObject* self, void* msg) {

	}

	void baseZonePropertyModelPlaced(Entity::GameObject* self, void* msg) {

	}

	void KillClouds(Entity::GameObject* self) {

	}

	void SpawnSpots(Entity::GameObject* self) {

	}

	void KillSpots(Entity::GameObject* self) {

	}

	void StartMaelstrom(Entity::GameObject* self, Entity::GameObject* player) {

	}

	void baseNotifyCollisionPhantom(Entity::GameObject* self, void* other, void* msg) {}

};
#endif 