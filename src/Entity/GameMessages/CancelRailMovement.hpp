#ifndef __ENTITY__GM__CancelRailMovement_HPP__
#define __ENTITY__GM__CancelRailMovement_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct CancelRailMovement : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_CANCEL_RAIL_MOVEMENT;
		}

		bool bImmediate;

		CancelRailMovement() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, bImmediate, false);
		}
	};
}

#endif
