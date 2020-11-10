#ifndef __GAMECACHE__OBJECTSKILLS_HPP__
#define __GAMECACHE__OBJECTSKILLS_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheObjectSkills {
	inline FDB::RowInfo getRow(int objectLOT) {
		FDB::RowTopHeader rth = Cache.getRows("ObjectSkills");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == objectLOT)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:ObjectSkills", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER_MR(0, std::int32_t, ObjectTemplate)
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(1, std::int32_t, SkillID)
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(2, std::int32_t, CastOnType)
	CRUX_CACHE_ADD_COLUMN_GETTER_MR(3, std::int32_t, AICombatWeight)
};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__
