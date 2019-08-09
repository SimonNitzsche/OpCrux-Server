#ifndef __GAMECACHE__PRECONDITIONS_HPP__
#define __GAMECACHE__PRECONDITIONS_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CachePreconditions {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("Preconditions");
		for(int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == id)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:Preconditions", e.what(), ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline int32_t GetType(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetTargetLOT(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[2]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetTargetGroup(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline int32_t GetTargetCount(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline int32_t GetIconID(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline bool GetLocalize(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation()) == 1;
	}

	inline int64_t GetValidContexts(int32_t id) {
		return *reinterpret_cast<int64_t*>(Cache.getFileData() + *reinterpret_cast<int32_t*>(getRow(id)/**/[7]/**/.getMemoryLocation()));
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[8]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetGateVersion(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[9]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__PRECONDITIONS_HPP__
