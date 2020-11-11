#ifndef __ENTITY__GM__VendorStatusUpdate_HPP__
#define __ENTITY__GM__VendorStatusUpdate_HPP__
#include "Entity/GameMessages.hpp"
#include "Missions/MissionManager.hpp"

namespace GM {
	struct VendorStatusUpdate : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_VENDOR_STATUS_UPDATE;
		}

		bool bUpdateOnly = false;

		struct VSU_InvItem { std::int32_t LOT; std::uint32_t sortPriority; };

		std::list<VSU_InvItem> inventoryList = {};

		VendorStatusUpdate() : GMBase() {}

		void Serialize(RakNet::BitStream* bs) {
			GM_VAR_SERIALIZE(bs, bUpdateOnly);
			
			bs->Write<std::uint32_t>(inventoryList.size());
			for (auto i : inventoryList) {
				bs->Write(i.LOT);
				bs->Write(i.sortPriority);
			}
		}
	};
}

#endif