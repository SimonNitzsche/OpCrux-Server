#ifndef __ENTITY__GM__PlayerLoaded_HPP__
#define __ENTITY__GM__PlayerLoaded_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct PlayerLoaded : GMBase {
		DataTypes::LWOOBJID playerID;

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_PLAYER_LOADED;
		}

		PlayerLoaded() {}
		void Deserialize(RakNet::BitStream * bs) {
			GM_VAR_DESERIALIZE(bs, playerID);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Entity::GameObject * player = sender->GetZoneInstance()->objectsManager->GetObjectByID(playerID);
			if(player != nullptr)
				Logger::log("WRLD", "["+ sender->GetNameStr() +"] Player " + player->GetNameStr() + " loaded.");
			else
				Logger::log("WRLD", "[" + sender->GetNameStr() + "] Unknown player " + std::to_string(playerID) + " loaded.");
		}
	};
}

#endif