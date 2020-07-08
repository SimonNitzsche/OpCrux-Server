#ifndef __GAMECACHE__RocketLaunchpadControlComponent_HPP__
#define __GAMECACHE__RocketLaunchpadControlComponent_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace RocketLaunchpadControlComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("RocketLaunchpadControlComponent");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (...) {}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, Id)
	CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, TargetZone)
	CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, DefaultZoneID)
	CRUX_CACHE_ADD_COLUMN_GETTER(3, FDB::PointerString, TargetScene)
	CRUX_CACHE_ADD_COLUMN_GETTER(4, std::int32_t, GmLevel)
	CRUX_CACHE_ADD_COLUMN_GETTER(5, FDB::PointerString, PlayerAninimation)
	CRUX_CACHE_ADD_COLUMN_GETTER(6, FDB::PointerString, RocketAninimation)
	CRUX_CACHE_ADD_COLUMN_GETTER(7, FDB::PointerString, LaunchMusic)
	CRUX_CACHE_ADD_COLUMN_GETTER(8, std::int32_t, UseLaunchPrecondition)
	CRUX_CACHE_ADD_COLUMN_GETTER(9, std::int32_t, UseAltLandingPrecondition)
	CRUX_CACHE_ADD_COLUMN_GETTER(10, FDB::PointerString, LaunchPrecondition)
	CRUX_CACHE_ADD_COLUMN_GETTER(11, FDB::PointerString, AltLandingPrecondition)
	CRUX_CACHE_ADD_COLUMN_GETTER(12, FDB::PointerString, AltLandingSpawnPointName)
};

#endif // !__Cache__RocketLaunchpadControlComponent_HPP__