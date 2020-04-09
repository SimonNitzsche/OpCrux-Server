#ifndef __ENTITY__GM__RequestDie_HPP__
#define __ENTITY__GM__RequestDie_HPP__
#include "Entity/GameMessages.hpp"
#include "Enums/EKillType.hpp"

namespace GM {
	struct RequestDie : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_REQUEST_DIE;
		}

		bool bDieAccepted;
		std::u16string deathType;
		std::float_t directionRelative_AngleXZ;
		std::float_t directionRelative_AngleY;
		std::float_t directionRelative_Force;
		Enums::EKillType killType = Enums::EKillType::VIOLENT;
		DataTypes::LWOOBJID killerID;
		DataTypes::LWOOBJID lootOwnerID;

		RequestDie() {}

		void Deserialize(RakNet::BitStream* bs) {
			GM_VAR_DESERIALIZE(bs, bDieAccepted);
			GM_VAR_DESERIALIZE_WSTRING(bs, deathType);
			GM_VAR_DESERIALIZE(bs, directionRelative_AngleXZ);
			GM_VAR_DESERIALIZE(bs, directionRelative_AngleY);
			GM_VAR_DESERIALIZE(bs, directionRelative_Force);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, killType, Enums::EKillType::VIOLENT);
			GM_VAR_DESERIALIZE(bs, killerID);
			GM_VAR_DESERIALIZE(bs, lootOwnerID);
		}

		inline void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			target->OnRequestDie(sender, *this);
		}
	};
}

#endif