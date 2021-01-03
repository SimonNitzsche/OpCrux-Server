#ifndef __ENTITY__NATIVESCRIPTS__AI_SPEC_L_SPECIAL_IMAGINE_POWERUP_SPAWNER__
#define __ENTITY__NATIVESCRIPTS__AI_SPEC_L_SPECIAL_IMAGINE_POWERUP_SPAWNER__

#include <string>
#include "Entity/NativeScript.hpp"
#include "Entity/GameObject.hpp"
#include "Entity/Components/CharacterComponent.hpp"

class NATIVESCRIPT__AI_SPEC_L_SPECIAL_IMAGINE_POWERUP_SPAWNER : public NativeScript {

	void onStartup(Entity::GameObject* self) {
		self->SetVar(u"bIsDead", false);
	}

	void onCollisionPhantom(Entity::GameObject* self, Entity::GameObject* other) {
		if (self->GetVar<bool>(u"bIsDead")) {
			return;
		}

		if (other->IsPlayer()) {
			CharacterComponent* comp = other->GetComponent<CharacterComponent>();

			comp->SetImagination(comp->GetImagination() + 1);

			GM::PlayFXEffect msg;
			msg.effectType = u"pickup";
			GameMessages::Send(other, self->GetObjectID(), msg);

			GM::Die nmsg;
			nmsg.killerID = other->GetObjectID();
			nmsg.killType = EKillType::SILENT;
			GameMessages::Send(other, self->GetObjectID(), nmsg);

			self->SetVar(u"bIsDead", true);
		}
	}
};

#endif 