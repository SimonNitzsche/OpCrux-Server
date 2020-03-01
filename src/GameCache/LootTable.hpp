#ifndef __GAMECACHE__LOOTTABLE_HPP__
#define __GAMECACHE__LOOTTABLE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheLootTable {
	inline FDB::RowInfo getRow(int32_t lootTableIndex) {
		FDB::RowTopHeader rth = Cache.getRows("LootTable");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[1].getMemoryLocation()) == lootTableIndex)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:LootTable", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, ItemID);
	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, LootTableIndex);
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, Id);
	CRUX_CACHE_ADD_COLUMN_GETTER(3, bool, MissionDrop);
	CRUX_CACHE_ADD_COLUMN_GETTER(4, std::int32_t, SortPriority);
};

#endif // !__GAMECACHE__LOOTMATRIX_HPP__
