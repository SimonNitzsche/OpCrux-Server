#ifndef __ENTITY__GM__Knockback_HPP__
#define __ENTITY__GM__Knockback_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct Knockback : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_KNOCKBACK;
		}

		DataTypes::LWOOBJID caster = 0;
		DataTypes::LWOOBJID originator = 0;
		std::int32_t iKnockBackTime = 0;
		DataTypes::Vector3 vector;

		Knockback() : GMBase() {}
		void Deserialize(RakNet::BitStream * bs) {

		}

		void Serialize(RakNet::BitStream * bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, caster, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, originator, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, iKnockBackTime, 0);
			GM_VAR_SERIALIZE(bs, vector.x);
			GM_VAR_SERIALIZE(bs, vector.y);
			GM_VAR_SERIALIZE(bs, vector.z);
		}

		void TriggerEvent(Entity::GameObject * sender, Entity::GameObject * target) {
			GameMessages::Send(sender->GetZoneInstance(), UNASSIGNED_SYSTEM_ADDRESS, target->GetObjectID(), *this);
		}
	};
}

#endif