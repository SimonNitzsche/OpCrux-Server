#ifndef __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_PLAYER_DEATH_TRIGGER_HPP__
#define __ENTITY__NATIVESCRIPTS__AI__AG__L_AG_SHIP_PLAYER_DEATH_TRIGGER_HPP__

#include <string>
#include "Entity/GameObject.hpp"
#include "Entity/NativeScript.hpp"

class NATIVESCRIPT__AI__AG__L_AG_SHIP_PLAYER_DEATH_TRIGGER : public NativeScript {
	std::u16string deathAnimation = u"electro-shock-death";
public:

	void onCollisionPhantom(Entity::GameObject* self) { // Collisions aren't implemented so /shrug
		if (self != nullptr) return;
		DataTypes::LWOOBJID objid = self->GetObjectID();
		GM::Die die;
		die.killerID = self->GetObjectID();
		die.deathType = deathAnimation;
		GameMessages::Send(self, objid, die);
	}
};

#endif 