#ifndef __ENTITY__GM__FireEventClientSide_HPP__
#define __ENTITY__GM__FireEventClientSide_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct FireEventClientSide : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_FIRE_CLIENT_EVENT;
		}

		std::u16string args;
		DataTypes::LWOOBJID object;
		std::int64_t param1 = 0;
		std::int32_t param2 = -1;
		DataTypes::LWOOBJID senderID;

		FireEventClientSide() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WSTRING(bs, args);
			GM_VAR_SERIALIZE(bs, object);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, param1, std::int64_t(0));
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, param2, std::int32_t(-1));
			GM_VAR_SERIALIZE(bs, senderID);
		}

	};
}

#endif