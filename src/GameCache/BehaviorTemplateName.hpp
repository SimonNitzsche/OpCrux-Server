#ifndef __GAMECACHE__BEHAVIORTEMPLATENAME_HPP__
#define __GAMECACHE__BEHAVIORTEMPLATENAME_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheBehaviorTemplateName {
	inline FDB::RowInfo getRow(int32_t templateID) {
		FDB::RowTopHeader rth = Cache.getRows("BehaviorTemplateName");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == templateID)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:BehaviorTemplateName", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	CRUX_CACHE_ADD_COLUMN_GETTER(0, std::int32_t, TemplateID)
	CRUX_CACHE_ADD_COLUMN_GETTER(1, FDB::PointerString, Name)

};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__
