#ifndef __ENTITY__GM__ToggleGhostReferenceOverride_HPP__
#define __ENTITY__GM__ToggleGhostReferenceOverride_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ToggleGhostReferenceOverride : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_TOGGLE_GHOST_REFERENCE_OVERRIDE;
		}

		ToggleGhostReferenceOverride() : GMBase() {}

		bool override;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, override, false);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			
		}
	};
}

#endif