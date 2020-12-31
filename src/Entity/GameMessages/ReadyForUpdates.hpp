#ifndef __ENTITY__GM__READYFORUPDATES_HPP__
#define __ENTITY__GM__READYFORUPDATES_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct ReadyForUpdates : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_READY_FOR_UPDATES;
		}

		DataTypes::LWOOBJID objectID;

		ReadyForUpdates() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, objectID);
		}

	};
}

#endif