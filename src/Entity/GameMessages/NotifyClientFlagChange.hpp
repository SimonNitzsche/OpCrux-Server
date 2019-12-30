#ifndef __ENTITY__GM__NotifyClientFlagChange_HPP__
#define __ENTITY__GM__NotifyClientFlagChange_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyClientFlagChange : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_SET_FLAG;
		}

		bool bFlag;
		std::int32_t iFlagID;

		NotifyClientFlagChange() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bFlag);
			GM_VAR_SERIALIZE(bs, iFlagID);
		}
	};
}

#endif