#ifndef __GAMECACHE__LOOTTABLE_HPP__
#define __GAMECACHE__LOOTTABLE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheLootTable {
	inline std::list<FDB::RowInfo> getRows(int32_t lootTableIndex) {
		FDB::RowTopHeader rth = Cache.getRows("LootTable");
		std::list<FDB::RowInfo> outRows = {};
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				FDB::RowInfo rowInfo = rth[i];
				while (rowInfo.isValid()) {
					if (*reinterpret_cast<int32_t*>(rowInfo[1].getMemoryLocation()) == lootTableIndex)
						outRows.push_back(rowInfo);

					if (rowInfo.isLinkedRowInfoValid()) {
						rowInfo = rowInfo.getLinkedRowInfo();
					}
					else {
						rowInfo = FDB::RowInfo();
					}
				}
			}
			catch (...) {}
		}
		return outRows;
	}

	CRUX_CACHE_ADD_COLUMN_GETTER_MR(0, std::int32_t, ItemID);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(1, std::int32_t, LootTableIndex);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(2, std::int32_t, Id);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(3, bool, MissionDrop);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(4, std::int32_t, SortPriority);
};

#endif // !__GAMECACHE__LOOTMATRIX_HPP__
