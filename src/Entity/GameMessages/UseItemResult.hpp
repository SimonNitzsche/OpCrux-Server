#ifndef __ENTITY__GM__UseItemResult_HPP__
#define __ENTITY__GM__UseItemResult_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct UseItemResult : GMBase {
		std::int32_t m_ItemTemplateID;
		bool m_UseItemResult = false;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_USED_ITEM_RESULT;
		}

		UseItemResult() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, m_ItemTemplateID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, m_UseItemResult, false);
		}
	};
}

#endif