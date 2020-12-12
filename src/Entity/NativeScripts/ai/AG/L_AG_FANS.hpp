#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_FANS_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_FANS_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScripts/OMIS.hpp"
#include "Utils/StringUtils.hpp"
#include "Entity/Components/RenderComponent.hpp"

class NATIVESCRIPT__AI__AG__L_AG_FANS : public NativeScript, public NATIVESCRIPT__OMIS {
public:
	void onStartup(Entity::GameObject* self) {
		self->SetVar(u"isOn", false);
		self->SetVar(u"isAlive", true);
		toggleFX(self);
		{ GM::PlayFXEffect msg; msg.name = "fanOn"; msg.effectID = 495; msg.effectType = u"fanOn"; self->CallMessage(msg); }
	}

	void toggleFX(Entity::GameObject* self, bool isHit = false) {
		std::u16string myGroup = self->GetVar<std::u16string>(u"groupID");
		StringUtils::replace(myGroup, u"%", u"");
		StringUtils::replace(myGroup, u";", u"");
		auto volumeGroup = self->GetZoneInstance()->objectsManager->GetObjectsInGroup(myGroup, 0ULL, true);

		if (volumeGroup.begin() == volumeGroup.end() || !self->GetVar(u"isAlive")) return;

		if (self->GetVar(u"isOn")) {
			{GM::PlayAnimation nmsg; nmsg.animationID = u"fan-off"; nmsg.bPlayImmediate = true; GameMessages::Broadcast(self, nmsg); }
			{GM::StopFXEffect nmsg; nmsg.name = "fanOn"; GameMessages::Broadcast(self, nmsg); }
			for (auto volGroupObject : volumeGroup) {
				auto volGroupObjectPhantom = volGroupObject->GetComponent<PhantomPhysicsComponent>();
				if (volGroupObjectPhantom != nullptr) volGroupObjectPhantom->SetEffectStatus(false);
				self->SetVar(u"isOn", false);
				if (!isHit) {
					{GM::PlayAnimation nmsg; nmsg.animationID = u"trigger"; nmsg.bPlayImmediate = true;
					GameMessages::Broadcast(self->GetZoneInstance()->objectsManager->GetObjectsInGroup(myGroup + u"fx", 0ULL, true).at(0), nmsg); }
				}
			}
		}
		else if (!self->GetVar(u"isOn") && self->GetVar(u"isAlive")) {
			{GM::PlayAnimation nmsg; nmsg.animationID = u"fan-on"; nmsg.bPlayImmediate = true; GameMessages::Broadcast(self, nmsg); }
			{GM::PlayFXEffect nmsg; nmsg.name = "fanOn"; nmsg.effectID = 495; nmsg.effectType = u"fanOn"; GameMessages::Broadcast(self, nmsg); }
			for (auto volGroupObject : volumeGroup) {
				auto volGroupObjectPhantom = volGroupObject->GetComponent<PhantomPhysicsComponent>();
				if(volGroupObjectPhantom != nullptr) volGroupObjectPhantom->SetEffectStatus(true);
				
				self->SetVar(u"isOn", true);
				if (!isHit) {
					{GM::PlayAnimation nmsg; nmsg.animationID = u"idle"; nmsg.bPlayImmediate = true;
					GameMessages::Broadcast(self->GetZoneInstance()->objectsManager->GetObjectsInGroup(myGroup + u"fx", 0ULL, true).at(0), nmsg); }
				}
			}
		}
	}

	void onFireEvent(Entity::GameObject* self, GM::FireEventServerSide msg) {
		if (msg.args.size() == 0 || !self->GetVar<bool>(u"isAlive")) return;
		auto fText = StringUtils::splitWString(msg.args, u',').at(0);
		if ((fText == u"turnOn" && self->GetVar(u"isOn")) || (fText == u"turnOff" && !self->GetVar(u"isOn"))) return;
		toggleFX(self);
	}

	void onDie(Entity::GameObject * self, GM::Die msg) {
		if (self->GetVar(u"isOn")) {
			toggleFX(self, true);
		}
		self->SetVar(u"isAlive", false);
	}
};
#endif

