#ifndef __ENTITY__GM__PlayerLoaded_HPP__
#define __ENTITY__GM__PlayerLoaded_HPP__
#include "Entity/Components/RacingControlComponent.hpp"


#include "Entity/GameMessages.hpp"

#include "Entity/GameMessages/RestoreToPostLoadStats.hpp"
#include "Entity/GameMessages/NotifyRacingClient.hpp"
#include "Entity/GameMessages/ActivityStart.hpp"

#include "PacketFactory/Chat/ChatPackets.hpp"

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

			GM::RestoreToPostLoadStats  rtpls;
			//GameMessages::Send(sender->GetZoneInstance(), sender->GetZoneInstance()->sessionManager.GetSession(sender->GetObjectID())->systemAddress, sender->GetObjectID(), rtpls);
			PacketFactory::Chat::SendChatMessage(sender->GetZoneInstance()->zoneControlObject, 4, u"Player " + sender->GetName() + u" joined the game.");

			auto zoneControlObject = sender->GetZoneInstance()->zoneControlObject;
			auto racingComp = zoneControlObject->GetComponent<RacingControlComponent>();
			if (racingComp != nullptr) {
				racingComp->msgPlayerAddedToWorldLocal(playerID);
			}

		}
	};
}

#endif