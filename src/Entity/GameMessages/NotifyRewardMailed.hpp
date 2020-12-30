#ifndef __ENTITY__GM__NotifyRewardMailed_HPP__
#define __ENTITY__GM__NotifyRewardMailed_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyRewardMailed : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_REWARD_MAILED;
		}

		DataTypes::LWOOBJID objectID;
		DataTypes::Vector3 startPoint;
		DataTypes::LWOOBJID subkey;
		std::int32_t templateID;

		NotifyRewardMailed() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, objectID);
			GM_VAR_SERIALIZE(bs, startPoint);
			GM_VAR_SERIALIZE(bs, subkey);
			GM_VAR_SERIALIZE(bs, templateID);
		}
	};
}

#endif