#ifndef __ENTITY__NATIVESCRIPTS__AI__FV__L_ACT_CANDLE_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__FV__L_ACT_CANDLE_HPP__

#include <string>
#include "Entity/NativeScript.hpp"
#include "Entity/GameMessages/PlayFXEffect.hpp"
#include "Entity/GameObject.hpp"

class NATIVESCRIPT__AI__FV__L_ACT_CANDLE : public NativeScript {
	// -----------------------------------------------------------------------------
	// --candle script to turn the effects off and on again
	// -----------------------------------------------------------------------------

	void onStartup(Entity::GameObject* self) {
		{GM::PlayFXEffect nmsg; nmsg.name = "candle_light"; nmsg.effectID = 2108; nmsg.effectType = u"create"; self->CallMessage(nmsg); }
		self->SetVar(u"Smoke", 5);
		self->SetVar(u"AmHit", false);
	}

	// TODO: implement other functions
};
#endif 