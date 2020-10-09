#ifndef __ENTITY__GM__Die_HPP__
#define __ENTITY__GM__Die_HPP__
#include "Entity/GameMessages.hpp"

#include "Enums/EKillType.hpp"

namespace GM {
	struct Die : public GMBase {
		bool bClientDeath = false;
		bool bDieAccepted = false;
		bool bSpawnLoot = true;
		std::float_t coinSpawnTime = 0.f;
		std::u16string deathType=u"";
		float directionRelative_AngleXZ=0;
		float directionRelative_AngleY=0;
		float directionRelative_Force=0;
		Enums::EKillType killType = Enums::EKillType::VIOLENT;
		DataTypes::LWOOBJID killerID;
		DataTypes::LWOOBJID lootOwnerID = DataTypes::LWOOBJID();

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_DIE;
		}

		Die() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE(bs, bClientDeath);
			GM_VAR_SERIALIZE(bs, bDieAccepted);
			GM_VAR_SERIALIZE(bs, bSpawnLoot);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, coinSpawnTime, 0.f);
			GM_VAR_SERIALIZE_WSTRING(bs, deathType);
			GM_VAR_SERIALIZE(bs, directionRelative_AngleXZ);
			GM_VAR_SERIALIZE(bs, directionRelative_AngleY);
			GM_VAR_DESERIALIZE(bs, directionRelative_Force);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, killType, Enums::EKillType::VIOLENT);
			GM_VAR_SERIALIZE(bs, killerID);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, lootOwnerID, DataTypes::LWOOBJID());
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			Logger::log("WRLD", "Triggered Die.");
		}
	};
}

#endif