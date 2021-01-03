#ifndef __ENTITY__GM__NotifyPetTamingMinigame_HPP__
#define __ENTITY__GM__NotifyPetTamingMinigame_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct NotifyPetTamingMinigame : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_NOTIFY_PET_TAMING_MINIGAME;
		}

		DataTypes::LWOOBJID PetID;
		DataTypes::LWOOBJID PlayerTamingID;
		bool bForceTeleport;
		uint32_t notifyType;
		Vector3 petsDestPos;
		Vector3 telePort;
		Quaternion teleRot = Quaternion();

		NotifyPetTamingMinigame() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, PetID);
			GM_VAR_SERIALIZE(bs, PlayerTamingID);
			GM_VAR_SERIALIZE(bs, bForceTeleport);
			GM_VAR_SERIALIZE(bs, notifyType);

			GM_VAR_SERIALIZE(bs, petsDestPos.x);
			GM_VAR_SERIALIZE(bs, petsDestPos.y);
			GM_VAR_SERIALIZE(bs, petsDestPos.z);

			GM_VAR_SERIALIZE(bs, telePort.x);
			GM_VAR_SERIALIZE(bs, telePort.y);
			GM_VAR_SERIALIZE(bs, telePort.z);

			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, teleRot.x, 0.f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, teleRot.y, 0.f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, teleRot.z, 0.f);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, teleRot.w, 1.f);
		}
	};
}

#endif