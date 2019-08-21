#ifndef __ENTITY__GM__RequestResurrect_HPP__
#define __ENTITY__GM__RequestResurrect_HPP__
#include "Entity/GameMessages.hpp"

#include "Entity/GameMessages/Resurrect.hpp"

namespace GM {
	struct RequestResurrect : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_RESURRECT;
		}

		RequestResurrect() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered RequestResurrect.");
			GM::Resurrect resurrectMsg = GM::Resurrect();

			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), resurrectMsg);
		}
	};
}

#endif