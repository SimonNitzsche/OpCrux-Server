#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SPACE_STUFF_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SPACE_STUFF_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SPACE_STUFF : public NativeScript {
	
public:

	void onStartup(Entity::GameObject * self) {
		self->GetZoneInstance()->timer.AddTimerWithCancelMs(5000, u"FloaterScale", self);
	}

	void onTimerDone(Entity::GameObject * self, GM::TimerDone msg) {
		if (msg.name == u"FloaterScale") {
			// Type of objects called
			int scaletype = 1 + rand() % ((5 + 1) - 1);  // Rand (1,5)

			// print("Scale " + std::to_string(scaletype));

			{ GM::PlayAnimation nmsg; nmsg.animationID = StringUtils::to_u16string("scale_0"+std::to_string(scaletype)); nmsg.TriggerEvent(self, self); }

			self->GetZoneInstance()->timer.AddTimerWithCancelMs(400, u"FloaterPath", self);
		}
		else if (msg.name == u"FloaterPath") {
			int pathtype = 1 + rand() % ((4 + 1) - 1); // Rand (1,4)
			int randtime = 20 + rand() % ((25 + 1) - 20); // Rand (20,25)

			// print("Move " + std::to_string(pathtype));

			{ GM::PlayAnimation nmsg; nmsg.animationID = StringUtils::to_u16string("path_0" + std::to_string(pathtype)); nmsg.TriggerEvent(self, self); }

			self->GetZoneInstance()->timer.AddTimerWithCancelMs(randtime * 1000, u"FloaterScale", self);
		}
	}


};
#endif 