#ifndef __ENTITY__GM__RequestSmashPlayer_HPP__
#define __ENTITY__GM__RequestSmashPlayer_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RequestSmashPlayer : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_SMASH_PLAYER;
		}

		RequestSmashPlayer() {}
		void Deserialize(RakNet::BitStream * bs) {
			
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered RequestSmashPlayer.");
			GM::Die dieMsg = GM::Die();
			dieMsg.bClientDeath = true;
			dieMsg.killerID = target->GetObjectID();
			dieMsg.lootOwnerID = target->GetObjectID();
			dieMsg.deathType = u"deaded";

			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), dieMsg);
		}
	};
}

#endif