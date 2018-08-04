#ifndef __GAMECACHE__LOOTMATRIX_HPP__
#define __GAMECACHE__LOOTMATRIX_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
extern FDB::Connection GameCache;

namespace CacheLootMatrix {
	inline FDB::RowInfo getRow(int32_t lootMatrixIndex) {
		FDB::RowTopHeader rth = GameCache.getRows("LootMatrix");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == lootMatrixIndex)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:LootMatrix", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline int32_t GetLootTableIndex(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetRarityTableIndex(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[2]/**/.getMemoryLocation());
	}

	inline float GetPercent(FDB::RowInfo row) {
		return *reinterpret_cast<float*>(row/**/[3]/**/.getMemoryLocation());
	}

	inline int32_t GetMinToDrop(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[4]/**/.getMemoryLocation());
	}

	inline int32_t GetMaxToDrop(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[5]/**/.getMemoryLocation());
	}

	inline int32_t GetID(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[6]/**/.getMemoryLocation());
	}

	inline int32_t GetFlagID(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[7]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetGateVersion(FDB::RowInfo row) {
		return FDB::PointerString(&GameCache, row/**/[8]/**/.getMemoryLocation());
	}
};
#endif