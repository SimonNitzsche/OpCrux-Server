#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_JET_EFFECT_SERVER_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_JET_EFFECT_SERVER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_JET_EFFECT_SERVER : public NativeScript {
public:
	void onUse(Entity::GameObject* self, GM::RequestUse msg) {
		if (self->GetLOT() == 6859 && !self->GetVar(u"isInUse")) {
			auto obj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"Jet_FX", 0Ui64, true).at(0);

			{GM::NotifyClientObject nmsg; nmsg.name = u"toggleInUse"; nmsg.param1 = 1; GameMessages::Broadcast(self, nmsg); }
			self->SetVar(u"isInUse", true);
			{GM::PlayAnimation nmsg; nmsg.animationID = u"jetFX"; GameMessages::Broadcast(self, nmsg); }
			{GM::PlayFXEffect nmsg; nmsg.name = "radarDish"; nmsg.effectType = u"create"; nmsg.effectID = 641; GameMessages::Broadcast(self, nmsg); }
			self->GetZoneInstance()->timer.AddTimerWithCancelMs(2000, u"radarDish", self);
			self->GetZoneInstance()->timer.AddTimerWithCancelMs(2500, u"PlayEffect", self);
			self->GetZoneInstance()->timer.AddTimerWithCancelMs(self->GetZoneInstance()->GetCinematicInfo(u"Jet_Cam_01") + 5000, u"CineDone", self);
		}
	}

	void onRebuildComplete(Entity::GameObject* self, GM::RebuildNotifyState msg) {
		if (self->GetLOT() == 6209) {
			auto obj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"Jet_FX", 0Ui64, true).at(0);
			auto objGroup = self->GetVar<std::u16string>(u"groupID");

			{GM::PlayAnimation nmsg; nmsg.animationID = u"jetFX"; GameMessages::Broadcast(obj, nmsg); }

			if (true /* We should check here for a groupID but one mortar is deactivated, leaving it out for now */) {
				self->GetZoneInstance()->timer.AddTimerWithCancelMs(2500, u"PlayEffect", self);
				self->GetZoneInstance()->timer.AddTimerWithCancelMs(self->GetZoneInstance()->GetCinematicInfo(u"Jet_Cam_01") + 5000, u"CineDone", self);
			}
		}
	}

	void onTimerDone(Entity::GameObject* self, GM::TimerDone msg) {
		if (msg.name == u"radarDish") {
			{GM::StopFXEffect nmsg; nmsg.name = StringUtils::to_string(msg.name); GameMessages::Broadcast(self, nmsg); }
		}
		else if (msg.name == u"PlayEffect") {
			// -- group name = mortarmain
			auto obj = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(u"mortarMain", 0Ui64, true);
			std::uniform_int_distribution<> testDist(0, obj.size());
			auto test = testDist(RandomUtil::GetEngine());
			if (test > 0) {
				{GM::CastSkill nmsg; nmsg.skillID = 318; obj.at(test - 1)->CallMessage(nmsg); }
			}
		}
		else if (msg.name == u"CineDone") {
			{GM::NotifyClientObject nmsg; nmsg.name = u"toggleInuse"; nmsg.param1 = -1; GameMessages::Broadcast(self, nmsg); }
			self->SetVar(u"isInUse", false);
		}
	}
};
#endif

