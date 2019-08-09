#ifndef __GAMECACHE__EMOTES_HPP__
#define __GAMECACHE__EMOTES_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheEmotes {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("Emotes");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:Emotes", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline FDB::PointerString GetAnimationName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetIconFilename(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetChannel(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetCommand(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline bool GetLocked(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[5]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetLocalize(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[7]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetGateVersion(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[8]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__EMOTES_HPP__
