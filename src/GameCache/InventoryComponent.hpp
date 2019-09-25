#ifndef __GAMECACHE__INVENTORYCOMPONENT_HPP__
#define __GAMECACHE__INVENTORYCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheInventoryComponent {
	inline FDB::RowInfo getRow(std::int32_t componentID) {
		FDB::RowTopHeader rth = Cache.getRows("InventoryComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == componentID)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:InventoryComponent", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline std::int32_t GetItemID(FDB::RowInfo row) {
		return *reinterpret_cast<std::int32_t*>(row/**/[1]/**/.getMemoryLocation());
	}

	inline std::int32_t GetCount(FDB::RowInfo row) {
		return *reinterpret_cast<std::int32_t*>(row/**/[2]/**/.getMemoryLocation());
	}

	inline bool GetEquip(FDB::RowInfo row) {
		return *reinterpret_cast<std::int32_t*>(row/**/[3]/**/.getMemoryLocation()) == 1;
	}
};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__
