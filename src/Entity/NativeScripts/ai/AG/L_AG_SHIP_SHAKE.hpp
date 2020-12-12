#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_SHAKE_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_SHAKE_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SHIP_SHAKE : public NativeScript {

	std::int32_t repTime = 2; // base time for the first shake
	std::int32_t randTime = 10; // number put into the math.random(), to get the next shake
	std::float_t shakeRad = 500; // radius away from object clients should shake
	Entity::GameObject * debrisObject = nullptr;
	// two different shake effect, only have one uncommented at a time
	std::u16string fxName = u"camshake-bridge"; // Lighter Shake with Angry animation
	// std::u16string fxName = L"camshake"; // Big Shake
public:

	float newTime() {
		float time = rand() % randTime;
		if (time < randTime * .5f) time = time + randTime * .5f;
		//print("newTime(): " + std::to_string(repTime + time));
		return repTime + time;
	}

	void onStartup(Entity::GameObject * self) {
		// See if object has some variables set in HF
		if (self->GetVar(u"MaxRepTime")) randTime = self->GetVar<std::int32_t>(u"MaxRepTime");
		if (self->GetVar(u"Radius")) shakeRad = self->GetVar<std::float_t>(u"Radius");
#ifdef OPCRUX_PLATFORM_WIN32
		if (self->GetVar(u"EffectName")) fxName = self->GetVar<std::u16string>(u"EffectName");
#else
		if (self->GetVar(u"EffectName")) fxName = self->GetVar<std::u16string>(u"EffectName");
#endif()
		debrisObject = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"DebrisFX").at(0);

		// Do the first Shake Timer on start up
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(repTime * 1000, u"ShakeTimer", self);
	}

	void onTimerDone(Entity::GameObject * self, GM::TimerDone msg) {
		// check to make sure there is a message and the timer name is correct
		Entity::GameObject * shipFXObj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"ShipFX").at(0);
		Entity::GameObject * shipFX2Obj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"ShipFX2").at(0);
		if (msg.name == u"ShakeTimer") {
			// Start another shake timer with newTime()
			self->GetZoneInstance()->timer.AddTimerWithCancel(newTime(), u"ShakeTimer", self);

			// Shakes the players using the variables at the top of the script
			//self : PlayEmbeddedEffectOnAllClientsNearObject{ radius = shakeRad, fromObjectID = self, effectName = fxName }
			{GM::PlayEmbeddedEffectOnAllClientsNearObject nmsg; nmsg.radius = shakeRad; nmsg.fromObjectID = self->GetObjectID(); nmsg.effectName = fxName; GameMessages::Broadcast(self->GetZoneInstance(), self, nmsg); }

			// Plays ship debris effect
			//debrisObj : PlayFXEffect{ name = "Debris", effectType = "DebrisFall" }
			{GM::PlayFXEffect nmsg; nmsg.name = "Debris"; nmsg.effectType = u"DebrisFall"; GameMessages::Broadcast(self->GetZoneInstance(), debrisObject, nmsg); }

			// Plays ship FX
			int randFX = rand() % 3;

			//shipFXObj : PlayFXEffect{ name = "FX", effectType = 'shipboom' ..randFX, effectID = 559 }
			{GM::PlayFXEffect nmsg; nmsg.name = "FX"; nmsg.effectType = StringUtils::to_u16string("shipboom" + std::to_string(randFX)); nmsg.effectID = 559; GameMessages::Broadcast(self->GetZoneInstance(), shipFXObj, nmsg); }
			//print("playFX **** " + std::to_string(randFX));
			//local animTime = shipFXObj : GetAnimationTime{ animationID = 'explosion' ..randFX }.time
			//GAMEOBJ : GetTimer() : AddTimerWithCancel(animTime, "ExplodeIdle", self)

			// Plays ship FX2
			//--shipFX2Obj : PlayFXEffect{ name = "FX2", effectType = 'ship-shake', effectID = 578 }
			//shipFX2Obj:PlayAnimation{ animationID = 'explosion' }
			{GM::PlayAnimation nmsg; nmsg.animationID = u"explosion"; GameMessages::Broadcast(self->GetZoneInstance(), shipFX2Obj, nmsg); }
		}
		else if (msg.name == u"ExplodeIdle") {
			// shipFXObj:PlayAnimation{ animationID = 'idle' }
			{GM::PlayAnimation nmsg; nmsg.animationID = u"idle"; GameMessages::Broadcast(self->GetZoneInstance(), shipFXObj, nmsg); }
			// shipFX2Obj:PlayAnimation{ animationID = 'idle' }
			{GM::PlayAnimation nmsg; nmsg.animationID = u"idle"; GameMessages::Broadcast(self->GetZoneInstance(), shipFX2Obj, nmsg); }
			//print("reset FX idle ****");
		}
	}


};
#endif 