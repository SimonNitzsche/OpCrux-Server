#ifndef __GAMECACHE__ZONETABLE_HPP__
#define __GAMECACHE__ZONETABLE_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheMovementAIComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("MovementAIComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:MovementAIComponent", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetMovementType(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline float GetWanderChance(int32_t id) {
		return *reinterpret_cast<std::float_t*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline float GetWanderDelayMin(int32_t id) {
		return *reinterpret_cast<std::float_t*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline float GetWanderDelayMax(int32_t id) {
		return *reinterpret_cast<std::float_t*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline float GetWanderSpeed(int32_t id) {
		return *reinterpret_cast<std::float_t*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline float GetWanderRadius(int32_t id) {
		return *reinterpret_cast<std::float_t*>(getRow(id)/**/[6]/**/.getMemoryLocation());
	}
};

#endif