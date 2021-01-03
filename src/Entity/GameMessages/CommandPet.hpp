#ifndef __ENTITY__GM__CommandPet_HPP__
#define __ENTITY__GM__CommandPet_HPP__
#include "Entity/GameMessages.hpp"

#include "Enums/EKillType.hpp"

namespace GM {
	struct CommandPet : public GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::CLIENT_COMMAND_PET;
		}

		Vector3 GenericPositionInfo;
		DataTypes::LWOOBJID ObjIDSource;
		int32_t iPetCommandType;
		int32_t iTypeID;
		bool overrideObey = false;

		CommandPet() : GMBase() {}

		void Deserialize(RakNet::BitStream* bs) {
			float x, y, z;
			GM_VAR_DESERIALIZE(bs, x);
			GM_VAR_DESERIALIZE(bs, y);
			GM_VAR_DESERIALIZE(bs, z);
			GenericPositionInfo = Vector3(x, y, z);

			GM_VAR_DESERIALIZE(bs, ObjIDSource);
			GM_VAR_DESERIALIZE(bs, iPetCommandType);
			GM_VAR_DESERIALIZE(bs, iTypeID);
			GM_VAR_DESERIALIZE_WITH_DEFAULT(bs, overrideObey, false);
		}
	};
}

#endif