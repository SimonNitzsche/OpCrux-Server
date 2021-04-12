#ifndef __GAMECACHE__MISSIONTASKS_HPP__
#define __GAMECACHE__MISSIONTASKS_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
#include "Enums/EMissionTask.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheMissionTasks {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("MissionTasks");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id)
						return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:MissionTasks", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::RowInfo getRowByTargetGroup(std::string TargetGroup) {
		FDB::RowTopHeader rth = Cache.getRows("MissionTasks");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (rowInfo.isValid())
					if (rowInfo[4].operator FDB::PointerString().operator std::string() == TargetGroup) return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:MissionTasks", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, ID)
	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, LocStatus)
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, TaskType)
	CRUX_CACHE_ADD_COLUMN_GETTER(3, std::int32_t, Target)
	CRUX_CACHE_ADD_COLUMN_GETTER(4, FDB::PointerString, TargetGroup)
	CRUX_CACHE_ADD_COLUMN_GETTER(5, std::int32_t, TargetValue)
	CRUX_CACHE_ADD_COLUMN_GETTER(6, FDB::PointerString, TaskParam1)
	CRUX_CACHE_ADD_COLUMN_GETTER(7, FDB::PointerString, LargeTaskIcon)
	CRUX_CACHE_ADD_COLUMN_GETTER(8, std::int32_t, IconID)
	CRUX_CACHE_ADD_COLUMN_GETTER(9, std::int32_t, UID)
	CRUX_CACHE_ADD_COLUMN_GETTER(10, std::int32_t, LargeTaskIconID)
	CRUX_CACHE_ADD_COLUMN_GETTER(11, bool, Localize)
	CRUX_CACHE_ADD_COLUMN_GETTER(12, FDB::PointerString, GateVersion)
};

#endif
