#ifndef __ENTITY__GM__NotifyRacingClient_HPP__
#define __ENTITY__GM__NotifyRacingClient_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"
#include "Enums/ERacingClientNotificationType.hpp"

namespace GM {
	struct NotifyRacingClient : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_RACING_CLIENT;
		}

		Enums::ERacingClientNotificationType eventType = Enums::ERacingClientNotificationType::INVALID;
		std::int32_t param1;
		DataTypes::LWOOBJID paramObj;
		std::u16string paramStr;
		DataTypes::LWOOBJID singleClient;

		NotifyRacingClient() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, eventType, Enums::ERacingClientNotificationType::INVALID);
			GM_VAR_SERIALIZE(bs, param1);
			GM_VAR_SERIALIZE(bs, paramObj);
			GM_VAR_SERIALIZE_WSTRING(bs, paramStr);
			GM_VAR_SERIALIZE(bs, singleClient);
		}
	};
}

#endif