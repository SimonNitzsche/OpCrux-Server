#ifndef __ENTITY__GM__RequestResurrect_HPP__
#define __ENTITY__GM__RequestResurrect_HPP__
#include "Entity/GameMessages.hpp"

#include "Entity/GameMessages/Resurrect.hpp"

namespace GM {
	struct RequestResurrect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_RESURRECT;
		}

		RequestResurrect() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

	};
}

#endif