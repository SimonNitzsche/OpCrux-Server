#ifndef __ENTITY__GM__NotifyClientObject_HPP__
#define __ENTITY__GM__NotifyClientObject_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyClientObject : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_CLIENT_OBJECT;
		}

		std::u16string name;
		std::int32_t param1 = 0;
		std::int32_t param2 = 0;
		DataTypes::LWOOBJID paramObj = std::uint64_t(0);
		std::string paramStr = "";

		NotifyClientObject() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WSTRING(bs, name);
			GM_VAR_SERIALIZE(bs, param1);
			GM_VAR_SERIALIZE(bs, param2);
			GM_VAR_SERIALIZE(bs, paramObj);
			GM_VAR_SERIALIZE_STRING(bs, paramStr);
		}

	};
}

#endif