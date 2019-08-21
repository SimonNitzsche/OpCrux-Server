#ifndef __ENTITY__GM__Resurrect_HPP__
#define __ENTITY__GM__Resurrect_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct Resurrect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_RESURRECT;
		}

		bool bRezImmediately=false;

		Resurrect() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, bRezImmediately);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {

		}
	};
}

#endif