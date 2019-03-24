#ifndef __GAMECACHE__SCRIPTCOMPONENT_HPP__
#define __GAMECACHE__SCRIPTCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection GameCache;

namespace CacheScriptComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = GameCache.getRows("ScriptComponent");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:ScriptComponent", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetScriptName(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetClientScriptName(int32_t id) {
		return FDB::PointerString(&GameCache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__SCRIPTCOMPONENT_HPP__
