#ifndef __ENTITY__GM__SETGHOSTREFERENCEPOSITION_HPP__
#define __ENTITY__GM__SETGHOSTREFERENCEPOSITION_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetGhostReferencePosition : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_SET_GHOST_REFERENCE_POSITION;
		}

		SetGhostReferencePosition() {}

		void Deserialize(RakNet::BitStream* bs) {

		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {

		}
	};
}

#endif