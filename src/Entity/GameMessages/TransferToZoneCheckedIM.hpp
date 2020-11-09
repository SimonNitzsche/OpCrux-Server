#ifndef __ENTITY__GM__TransferToZoneCheckedIM_HPP__
#define __ENTITY__GM__TransferToZoneCheckedIM_HPP__

#include "Entity/GameMessages.hpp"
#undef max
#include <limits>
#include <iostream>

namespace GM {
	struct TransferToZoneCheckedIM : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_TRANSFER_TO_ZONE_CHECKED_IM;
		}

		bool bIsThereaQueue = false;
		std::uint32_t cloneID = 0;
		std::float_t pos_x = std::numeric_limits<std::float_t>::max();
		std::float_t pos_y = std::numeric_limits<std::float_t>::max();
		std::float_t pos_z = std::numeric_limits<std::float_t>::max();
		std::float_t rot_w = 1;
		std::float_t rot_x = 0;
		std::float_t rot_y = 0;
		std::float_t rot_z = 0;
		std::u16string spawnPoint = u"";
		std::uint8_t ucInstanceType = 0;
		std::uint16_t zoneID = -1;

		TransferToZoneCheckedIM() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, bIsThereaQueue, false);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, cloneID, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, pos_x, std::numeric_limits<std::float_t>::max());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, pos_y, std::numeric_limits<std::float_t>::max());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, pos_z, std::numeric_limits<std::float_t>::max());
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, rot_w, 1);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, rot_x, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, rot_y, 0);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, rot_z, 0);
			GM_VAR_SERIALIZE_WSTRING(bs, spawnPoint);
			GM_VAR_SERIALIZE(bs, ucInstanceType);
			GM_VAR_SERIALIZE_WITH_DEFAULT(bs, zoneID, -1);

		}
	};
}

#endif