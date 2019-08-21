#ifndef __ENTITY__GM__SetGravityScale_HPP__
#define __ENTITY__GM__SetGravityScale_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct SetGravityScale : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_GRAVITY_SCALE;
		}

		float gravityScale=1;

		SetGravityScale() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, gravityScale);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			
		}
	};
}

#endif