#ifndef __ENTITY__GM__ReportBug_HPP__
#define __ENTITY__GM__ReportBug_HPP__
#include "Entity/GameMessages.hpp"

#include "Enums/EKillType.hpp"

namespace GM {
	struct ReportBug : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_HELPMENU_REQUEST_HELP;
		}

		ReportBug() : GMBase() {}

		std::u16string body;
		std::string clientVersion;
		std::string nOtherPlayerID;
		std::string selection;

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE_WSTRING(bs, body);
			GM_VAR_DESERIALIZE_STRING(bs, clientVersion);
			GM_VAR_DESERIALIZE_STRING(bs, nOtherPlayerID);
			GM_VAR_DESERIALIZE_STRING(bs, selection);
		}

		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			std::string bodybutnotunicode = StringUtils::to_string(body);
			Logger::log("WRLD", "A report has come in here is the details", LogType::WARN);
			std::string message = "\t\tBody: " + bodybutnotunicode;
			Logger::log("WRLD", message, LogType::WARN);
			std::string message2 = "\t\tOtherPlayerID: " + nOtherPlayerID;
			if (StringUtils::StringToInt(nOtherPlayerID) != 0) { Logger::log("WRLD", message2, LogType::WARN); }
		}

	};
}

#endif