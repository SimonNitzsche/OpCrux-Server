#ifndef __ENTITY__GM__OfferMission_HPP__
#define __ENTITY__GM__OfferMission_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct OfferMission : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_OFFER_MISSION;
		}

		std::int32_t missionID;
		DataTypes::LWOOBJID offerer;

		OfferMission() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, missionID);
			GM_VAR_SERIALIZE(bs, offerer);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {

		}
	};
}

#endif