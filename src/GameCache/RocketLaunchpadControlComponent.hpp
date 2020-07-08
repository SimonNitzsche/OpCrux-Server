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

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, efId)
		CRUX_CACHE_ADD_COLUMN_GETTER(1, std::int32_t, id)
		CRUX_CACHE_ADD_COLUMN_GETTER(2, std::int32_t, targetZone)
		CRUX_CACHE_ADD_COLUMN_GETTER(3, std::int32_t, defaultZoneID)
		CRUX_CACHE_ADD_COLUMN_GETTER(4, FDB::PointerString, targetScene)
		CRUX_CACHE_ADD_COLUMN_GETTER(5, std::int32_t, gmLevel)
		CRUX_CACHE_ADD_COLUMN_GETTER(6, FDB::PointerString, playerAninimation)
		CRUX_CACHE_ADD_COLUMN_GETTER(7, FDB::PointerString, rocketAninimation)
		CRUX_CACHE_ADD_COLUMN_GETTER(8, FDB::PointerString, launchMusic)
		CRUX_CACHE_ADD_COLUMN_GETTER(9, std::int32_t, useLaunchPrecondition)
		CRUX_CACHE_ADD_COLUMN_GETTER(10, std::int32_t, useAltLandingPrecondition)
		CRUX_CACHE_ADD_COLUMN_GETTER(11, FDB::PointerString, launchPrecondition)
		CRUX_CACHE_ADD_COLUMN_GETTER(12, FDB::PointerString, altLandingPrecondition)
		CRUX_CACHE_ADD_COLUMN_GETTER(13, FDB::PointerString, altLandingSpawnPointName)
};

#endif // !__Cache__RocketLaunchpadControlComponent_HPP__