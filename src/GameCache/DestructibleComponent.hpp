#ifndef __GAMECACHE__DESTRUCTIBLECOMPONENT_HPP__
#define __GAMECACHE__DESTRUCTIBLECOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CacheDestructibleComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("DestructibleComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				FDB::RowInfo rowInfo = rth[i];
				while (rowInfo.isValid()) {
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
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
		return FDB::RowInfo();
	}

	inline int32_t GetFaction(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetFactionList(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t GetLife(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline int32_t GetImagination(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline int32_t GetLootMatrixIndex(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline int32_t GetCurrencyIndex(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation());
	}

	inline int32_t GetLevel(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[7]/**/.getMemoryLocation());
	}

	inline float GetArmor(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[8]/**/.getMemoryLocation());
	}

	inline int32_t GetDeathBehavior(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[9]/**/.getMemoryLocation());
	}

	inline bool GetIsNPC(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[10]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetAttackPriority(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[11]/**/.getMemoryLocation());
	}

	inline bool GetIsSmashable(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[12]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetDifficultyLevel(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__DESTRUCTIBLECOMPONENT_HPP__
