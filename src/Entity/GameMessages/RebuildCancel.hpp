#ifndef __ENTITY__GM__RebuildCancel_HPP__
#define __ENTITY__GM__RebuildCancel_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RebuildCancel : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_CANCEL_REBUILD;
		}

		bool bEarlyRelease;
		DataTypes::LWOOBJID userID;

		RebuildCancel() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, bEarlyRelease);
			GM_VAR_DESERIALIZE(bs, userID);
		}
	};
}

#endif