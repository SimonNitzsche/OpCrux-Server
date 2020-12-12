#ifndef __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__AG__L_AG_MONUMENT_BIRDS_HPP__
#define __ENTITY__NATIVESCRIPTS__02_SERVER__MAP__AG__L_AG_MONUMENT_BIRDS_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"
#include "Entity/Components/DestructibleComponent.hpp"
#include <memory>

class NATIVESCRIPT__02_SERVER__MAP__AG__L_AG_MONUMENT_BIRDS : public NativeScript {

	const std::u16string sOnProximityAnim = u"fly1";
	const std::float_t flyRadius = 5;

public:

	void onStartup(Entity::GameObject* self) {
		// set up the proximity radius, and pulse the proximity
		self->SetProximityRadius("MonumentBirds", flyRadius);
	}

	void onProximityUpdate(Entity::GameObject* self, GM::ProximityUpdate msg) {
		if (self->GetVar(u"bAnimating")) return;

		// is this the correct proximity
		if (msg.name == "MonumentBirds" && msg.status == "ENTER") {
			self->SetVar(u"bAnimating", true);

			auto animTime = 1.75f;

			if (animTime == 0) {
				animTime = 1;
			}

			self->GetZoneInstance()->timer.AddTimerWithCancelMs(animTime * 1000, StringUtils::to_u16string(std::to_string(msg.objId->GetObjectID())), self);

			{GM::PlayAnimation nmsg; nmsg.animationID = sOnProximityAnim; nmsg.fPriority = 4.0f; nmsg.bPlayImmediate = true; GameMessages::Broadcast(self, nmsg); }

		}
	}

	void onTimerDone(Entity::GameObject* self, GM::TimerDone msg) {
		auto player = self->GetZoneInstance()->objectsManager->GetObjectByID(std::stoull(StringUtils::to_string(msg.name)));

		if (player != nullptr)
		{
			GM::RequestDie nmsg; nmsg.killerID = player->GetObjectID(); self->GetComponent<DestructibleComponent>()->ForcefullyPerformRequestDie(self, nmsg);
		}
	}

};
#endif 