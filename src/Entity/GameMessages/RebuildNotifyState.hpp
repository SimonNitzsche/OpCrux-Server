#ifndef __ENTITY__GM__RebuildNotifyState_HPP__
#define __ENTITY__GM__RebuildNotifyState_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RebuildNotifyState : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_REBUILD_NOTIFY_STATE;
		}

		std::int32_t iPrevState;
		std::int32_t iState;
		DataTypes::LWOOBJID player;

		RebuildNotifyState() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, iPrevState);
			GM_VAR_SERIALIZE(bs, iState);
			GM_VAR_SERIALIZE(bs, player);
		}
	};
}

#endif