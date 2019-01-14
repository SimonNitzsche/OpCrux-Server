#ifndef __GAMECACHE__MOVINGPLATFORMS_HPP__
#define __GAMECACHE__MOVINGPLATFORMS_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CacheMovingPlatforms {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("MovingPlatforms");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:MovingPlatforms", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline bool GetPlatformIsSimpleMover(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[1]/**/.getMemoryLocation()) == 1;
	}

	inline float GetPlatformMoveX(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline float GetPlatformMoveY(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline float GetPlatformMoveZ(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline float GetPlatformMoveTime(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline bool GetPlatformStartAtEnd(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString GetDescription(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[7]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__MOVINGPLATFORMS_HPP__
