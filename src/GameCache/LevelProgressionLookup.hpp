#ifndef __GAMECACHE__LEVELPROGRESSIONLOOKUP_HPP__
#define __GAMECACHE__LEVELPROGRESSIONLOOKUP_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheLevelProgrssionLookup {
	inline FDB::RowInfo getRow(int32_t level) {
		FDB::RowTopHeader rth = Cache.getRows("LevelProgressionLookup");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == level)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:LevelProgressionLookup", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER_MR(0, std::int32_t, ID);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(1, std::int32_t, RequiredUScore);
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(2, std::int32_t, BehaviorEffect);

	inline FDB::RowInfo GetRowLevelByUScorePassed(std::int32_t uscore) {
		FDB::RowTopHeader rth = Cache.getRows("LevelProgressionLookup");
		FDB::RowInfo lastPassed;
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][1].getMemoryLocation()) > uscore)
					break;
				lastPassed = rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:LevelProgressionLookup", e.what(), LogType::ERR);
			}
		}
		return lastPassed;
	}

	inline std::int32_t GetLevelByUScorePassed(std::int32_t uscore) {
		FDB::RowInfo row = GetRowLevelByUScorePassed(uscore);
		if (row.isValid())
			return row[0];
		return 1;
	}
};

#endif // !__GAMECACHE__LEVELPROGRESSIONLOOKUP_HPP__
