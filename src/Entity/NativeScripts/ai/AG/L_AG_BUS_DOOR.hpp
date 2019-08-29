#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_BUS_DOOR_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_BUS_DOOR_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_BUS_DOOR : public NativeScript {
	//---- Moves the bus door while players are close to it.
	//---- Updated: 7/01/09 jnf...
	//---- Updated: 5/19/10 djames... added second proxy radius and outerCounter
	//
	//-- the logic behind opening and closing the door is based on two proximity radii
	//-- it is possible for players to leave the proximity sphere unintentionally 
	//-- when the bus moves up because the sphere moves with the bus
	//-- by maintaining two counters, for both the inner and outer radii, players will
	//-- only be considered "in" proximity if they are inside both radii and they will
	//-- only be considered "out" of proximity if they are outside both radii
	//
	int ProxRadius = 75; // the radius for "inner" proximity detection
	int OuterProxRadius = 85; // the radius for "outer" proximity detection
	std::string soundName = "{9a24f1fa-3177-4745-a2df-fbd996d6e1e3}";

	void onStartup(Entity::GameObject * self) {
		self->SetVar(L"counter", 0);
		self->SetVar(L"outerCounter", 0);
		self->SetProximityRadius("busDoor", ProxRadius);
		self->SetProximityRadius("busDoorOuter", OuterProxRadius);
		//self:StopPathing()
	}

	void moveDoor(Entity::GameObject * self, bool bOpen) {
		// print("move door **************");

		if (bOpen) {
			print("open door **************");
			//self:GoToWaypoint{iPathIndex = 0}
		}
		else {
			print("close door **************");
			//self:GoToWaypoint{iPathIndex = 1}               
		}

		self->PlayNDAudioEmitter(soundName);

	}

	void onProximityUpdate(Entity::GameObject * self, GM::ProximityUpdate msg) {

		// If this isn't the proximity radius for the bus door behavior, then we're done here
		if (msg.name != "busDoor" && msg.name != "busDoorOuter") return;
		//    
		//    -- Make sure only humans are taken into account
		//    if not msg.objId:BelongsToFaction{factionID = 1}.bIsInFaction then return end 
		//            

		if (msg.objId->GetLOT() != 1) return;

		int counter = self->GetVar(L"counter");
		int outerCounter = self->GetVar(L"outerCounter");

		if (msg.status == "ENTER") {
			if (msg.name == "busDoor") {
				++counter;
			}
			else if (msg.name == "busDoorOuter") {
				++outerCounter;
			}
			// move up when a player is inside both radii
			if (counter == 1 && outerCounter >= 1) {
				moveDoor(self, true);
			}
		}
		else if (msg.status == "LEAVE") {
			if (msg.name == "busDoor") {
				if (counter > 0) {
					--counter;
				}
			}
			else if (msg.name == "busDoorOuter") {
				if (outerCounter > 0) {
					--outerCounter;
					int ceds = 43;
				}
			}

			// move down when no players are inside either radii
			if (counter == 0 && outerCounter == 0) {
				moveDoor(self, false);
			}

		}
		//	
		// print("proximity update: status="+msg.status+", name="+msg.name+", counter="+std::to_string(counter)+", outerCounter="+std::to_string(outerCounter));
		//    
		self->SetVar(L"counter", counter);
		self->SetVar(L"outerCounter", outerCounter);
	}

	void onArrivedAtDesiredWaypoint(Entity::GameObject * self, GM::ArrivedAtDesiredWaypoint msg) {
		if (msg.iPathIndex == 1) {
			// self:PlayFXEffect{ name  = "busDust", effectType = "create"} -- effectID = 642,
			// print('play effect busDoor');
		}
	}
};
#endif 