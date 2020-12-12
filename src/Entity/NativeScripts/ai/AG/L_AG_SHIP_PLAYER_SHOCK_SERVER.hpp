#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_SHOCK_SERVER : public NativeScript {
	//----------------------------------------------------------
	//plays the shock animation on the player when they interact with the broken console on the space ship
	//----------------------------------------------------------


	std::u16string ShockAnim = u"knockback-recovery";
	int FXTime = 2000;
public:

	void onStartup(Entity::GameObject * self) {
		self->SetVar(u"bActive", false);
	}

	void onUse(Entity::GameObject * self, GM::RequestUse msg) {
		print("clicked!");
		//msg.user:TerminateInteraction{ type = 'fromInteraction', ObjIDTerminator = self }

		if (self->GetVar(u"bActive")) {
			return;
		}

		auto player = msg.user;
		self->SetVar(u"bActive", true);

		{ GM::PlayAnimation nmsg; nmsg.animationID = ShockAnim; nmsg.bPlayImmediate = true; nmsg.TriggerEvent(self, player); }
		{ GM::Knockback nmsg; nmsg.vector = Vector3(-20, 10, -20); nmsg.TriggerEvent(self, player); }
		{ GM::PlayFXEffect nmsg; nmsg.name = "console_sparks"; nmsg.effectType = u"create"; nmsg.effectID = 1430; nmsg.TriggerEvent(self, self); }

		self->GetZoneInstance()->timer.AddTimerWithCancelMs(FXTime, u"FXTime", self);
	}
	void onTimerDone(Entity::GameObject * self, GM::TimerDone msg) {
		{ GM::StopFXEffect nmsg; nmsg.name = "console_sparks"; nmsg.TriggerEvent(self, self); }
		self->SetVar(u"bActive", false);
	}
	
	
};
#endif 