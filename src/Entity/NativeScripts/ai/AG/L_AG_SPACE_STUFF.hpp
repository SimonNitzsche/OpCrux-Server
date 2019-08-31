#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SPACE_STUFF_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SPACE_STUFF_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SPACE_STUFF : public NativeScript {
	
public:

	void onStartup(Entity::GameObject * self) {
		self->GetZoneInstance()->timer.AddTimerWithCancel(5000, L"FloaterScale", self);
	}

	void onTimerDone(Entity::GameObject * self, TimerDone msg) {
		if (msg.name == L"FloaterScale") {
			// Type of objects called
			int scaletype = 1 + rand() % ((5 + 1) - 1);  // Rand (1,5)

			print("Scale " + std::to_string(scaletype));

			{ GM::PlayAnimation nmsg; nmsg.animationID = L"scale_0"+std::to_wstring(scaletype); nmsg.TriggerEvent(self, self); }

			self->GetZoneInstance()->timer.AddTimerWithCancel(400, L"FloaterPath", self);
		}
		else if (msg.name == L"FloaterPath") {
			int pathtype = 1 + rand() % ((4 + 1) - 1); // Rand (1,4)
			int randtime = 20 + rand() % ((25 + 1) - 20); // Rand (20,25)

			print("Move " + std::to_string(pathtype));

			{ GM::PlayAnimation nmsg; nmsg.animationID = L"path_0" + std::to_wstring(pathtype); nmsg.TriggerEvent(self, self); }

			self->GetZoneInstance()->timer.AddTimerWithCancel(randtime * 1000, L"FloaterScale", self);
		}
	}


};
#endif 