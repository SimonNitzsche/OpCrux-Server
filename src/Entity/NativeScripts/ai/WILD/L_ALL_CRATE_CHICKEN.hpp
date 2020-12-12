#ifndef __ENTITY__NATIVESCRIPTS__AI__WILD__L_ALL_CRATE_CHICKEN_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__WILD__L_ALL_CRATE_CHICKEN_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__WILD__L_ALL_CRATE_CHICKEN : public NativeScript {
public:

	void onStartup(Entity::GameObject* self) {
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(4200, u"KillRooster", self);
	}

	void onTimerDone(Entity::GameObject* self, GM::TimerDone msg) {
		if (msg.name == u"KillRooster") {
			{
				GM::Die nmsg; nmsg.killerID = self->GetObjectID(); nmsg.killType = Enums::EKillType::SILENT;
				GameMessages::Broadcast(self, nmsg);
				self->OnDie(self, &nmsg);
				self->InstantiateRemoval();
			}
		}
	}

};
#endif 