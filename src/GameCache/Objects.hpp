#ifndef __GAMECACHE__OBJECTS_HPP__
#define __GAMECACHE__OBJECTS_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheObjects {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("Objects");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
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

	inline FDB::PointerString GetName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline bool GetPlacable(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[2]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString GetType(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetDescription(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline bool GetLocalize(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[5]/**/.getMemoryLocation()) == 1;
	}

	inline int32_t GetNPCTemplate(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[6]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetDisplayName(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[7]/**/.getMemoryLocation());
	}

	inline float GetInteractionDistance(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[8]/**/.getMemoryLocation());
	}

	inline bool GetNametag(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[9]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString Get_InternalNotes(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[10]/**/.getMemoryLocation());
	}

	inline int32_t GetLocaleStatus(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[11]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetGateVersion(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[12]/**/.getMemoryLocation());
	}

	inline bool GetHQ_Valid(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[13]/**/.getMemoryLocation()) == 1;
	}
};

#endif // !__GAMECACHE__OBJECTS_HPP__
