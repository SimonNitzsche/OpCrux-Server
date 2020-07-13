#ifndef __ENTITY__GM__BUILDEXITCONFIRMATION_HPP__
#define __ENTITY__GM__BUILDEXITCONFIRMATION_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct BuildExitConfirmation : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_BUILD_EXIT_CONFIRMATION;
		}

		BuildExitConfirmation() : GMBase() {}

		DataTypes::LWOOBJID playerID;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, playerID);
		}

	};
}

#endif