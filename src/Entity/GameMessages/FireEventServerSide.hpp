#ifndef __ENTITY__GM__FireEventServerSide_HPP__
#define __ENTITY__GM__FireEventServerSide_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct FireEventServerSide : GMBase {
		std::wstring args = L"";
		std::int32_t param1 = -1;
		std::int32_t param2 = -1;
		std::int32_t param3 = -1;
		DataTypes::LWOOBJID senderID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_FIRE_EVENT_SERVER_SIDE;
		}

		FireEventServerSide() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {
			
			GM_VAR_DESERIALIZE_WSTRING(bs, args);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, param1, -1);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, param2, -1);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, param3, -1);
			GM_VAR_DESERIALIZE(bs, senderID);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered FireEventServerSide with " + std::string(args.begin(), args.end()), LogType::PASSED);
		}
	};
}

#endif