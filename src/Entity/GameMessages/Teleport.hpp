#ifndef __ENTITY__GM__Teleport_HPP__
#define __ENTITY__GM__Teleport_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct Teleport : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_TELEPORT;
		}

		Teleport() : GMBase() {}

		bool NoGravTeleport;
		bool bIgnoreY = true;
		bool bSetRotation = false;
		bool bSkipAllChecks = false;
		DataTypes::Vector3 pos;
		bool useNavmesh = false;
		float w = 1.0;
		float x;
		float y;
		float z;
		DataTypes::Quaternion rotation = DataTypes::Quaternion(x, y, z, w);

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, NoGravTeleport);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bIgnoreY, true);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bSetRotation, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bSkipAllChecks, false);
			GM_VAR_SERIALIZE(bs, pos);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, useNavmesh, false);
			GM_VAR_SERIALIZE(bs, rotation);
		}



		void TriggerEvent(Entity::GameObject* sender, Entity::GameObject* target) {
			Logger::log("WRLD", "Triggered Teleport");
		}
	};
}

#endif