#ifndef __ENTITY__GM__ModifyGhostingDistance_HPP__
#define __ENTITY__GM__ModifyGhostingDistance_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ModifyGhostingDistance : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_MODIFY_GHOSTING_DISTANCE;
		}

		ModifyGhostingDistance() : GMBase() {}

		float fDistanceScalar;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, fDistanceScalar, 1.0f);
		}
	};
}

#endif