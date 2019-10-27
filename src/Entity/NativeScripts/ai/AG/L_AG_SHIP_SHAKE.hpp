#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_SHAKE_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_SHAKE_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SHIP_SHAKE : public NativeScript {

	int repTime = 2; // base time for the first shake
	int randTime = 10; // number put into the math.random(), to get the next shake
	float shakeRad = 500; // radius away from object clients should shake
	Entity::GameObject * debrisObject = nullptr;
	// two different shake effect, only have one uncommented at a time
	std::wstring fxName = L"camshake-bridge"; // Lighter Shake with Angry animation
	// std::wstring fxName = L"camshake"; // Big Shake
public:

	float newTime() {
		float time = rand() % randTime;
		if (time < randTime * .5f) time = time + randTime * .5f;
		print("newTime(): " + std::to_string(repTime + time));
		return repTime + time;
	}

	void onStartup(Entity::GameObject * self) {
		// See if object has some variables set in HF
		if (self->GetVar(L"MaxRepTime")) randTime = self->GetVar(L"MaxRepTime");
		if (self->GetVar(L"Radius")) shakeRad = self->GetVar(L"Radius");
#ifdef OPCRUX_PLATFORM_WIN32
		if (self->GetVar(L"EffectName")) fxName = self->GetVar(L"EffectName");
#else
		if (self->GetVar(L"EffectName")) fxName = self->GetVar(L"EffectName");
#endif()
		debrisObject = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(L"DebrisFX").at(0);

		// Do the first Shake Timer on start up
		self->GetZoneInstance()->timer.AddTimerWithCancel(repTime * 1000, L"ShakeTimer", self);
	}

	void onTimerDone(Entity::GameObject * self, TimerDone msg) {
		// check to make sure there is a message and the timer name is correct
		Entity::GameObject * shipFXObj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(L"ShipFX").at(0);
		Entity::GameObject * shipFX2Obj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(L"ShipFX2").at(0);
		if (msg.name == L"ShakeTimer") {
			// Start another shake timer with newTime()
			self->GetZoneInstance()->timer.AddTimerWithCancel(newTime(), L"ShakeTimer", self);

			// Shakes the players using the variables at the top of the script
			//self : PlayEmbeddedEffectOnAllClientsNearObject{ radius = shakeRad, fromObjectID = self, effectName = fxName }
			{GM::PlayEmbeddedEffectOnAllClientsNearObject nmsg; nmsg.radius = shakeRad; nmsg.fromObjectID = self->GetObjectID(); nmsg.effectName = fxName; GameMessages::Broadcast(self->GetZoneInstance(), self, nmsg); }

			// Plays ship debris effect
			//debrisObj : PlayFXEffect{ name = "Debris", effectType = "DebrisFall" }
			{GM::PlayFXEffect nmsg; nmsg.name = "Debris"; nmsg.effectType = L"DebrisFall"; GameMessages::Broadcast(self->GetZoneInstance(), debrisObject, nmsg); }

			// Plays ship FX
			int randFX = rand() % 3;

			//shipFXObj : PlayFXEffect{ name = "FX", effectType = 'shipboom' ..randFX, effectID = 559 }
			{GM::PlayFXEffect nmsg; nmsg.name = "FX"; nmsg.effectType = L"shipboom" + std::to_wstring(randFX); nmsg.effectID = 559; GameMessages::Broadcast(self->GetZoneInstance(), shipFXObj, nmsg); }
			print("playFX **** " + std::to_string(randFX));
			//local animTime = shipFXObj : GetAnimationTime{ animationID = 'explosion' ..randFX }.time
			//GAMEOBJ : GetTimer() : AddTimerWithCancel(animTime, "ExplodeIdle", self)

			// Plays ship FX2
			//--shipFX2Obj : PlayFXEffect{ name = "FX2", effectType = 'ship-shake', effectID = 578 }
			//shipFX2Obj:PlayAnimation{ animationID = 'explosion' }
			{GM::PlayAnimation nmsg; nmsg.animationID = L"explosion"; GameMessages::Broadcast(self->GetZoneInstance(), shipFX2Obj, nmsg); }
		}
		else if (msg.name == L"ExplodeIdle") {
			// shipFXObj:PlayAnimation{ animationID = 'idle' }
			{GM::PlayAnimation nmsg; nmsg.animationID = L"idle"; GameMessages::Broadcast(self->GetZoneInstance(), shipFXObj, nmsg); }
			// shipFX2Obj:PlayAnimation{ animationID = 'idle' }
			{GM::PlayAnimation nmsg; nmsg.animationID = L"idle"; GameMessages::Broadcast(self->GetZoneInstance(), shipFX2Obj, nmsg); }
			print("reset FX idle ****");
		}
	}


};
#endif 