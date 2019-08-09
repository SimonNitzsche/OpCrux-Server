#ifndef __GAMECACHE__OBJECTS_HPP__
#define __GAMECACHE__OBJECTS_HPP__

#include "Interface/FastDatabase.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheComponentsRegistry {
	inline FDB::RowInfo getRowByType(int32_t id, int32_t component_type) {
		FDB::RowTopHeader rth = Cache.getRows("ComponentsRegistry");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				FDB::RowInfo rowInfo = rth[i];
				if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id) {
					while (rowInfo.isValid()) {
						if (*reinterpret_cast<int32_t*>(rowInfo[1].getMemoryLocation()) == component_type)
							return rth[i];
						if (rowInfo.isLinkedRowInfoValid()) {
							rowInfo = rowInfo.getLinkedRowInfo();
						}
						else {
							rowInfo = FDB::RowInfo();
						}
					}
				}
			}
			catch (...) {}
		}
		return FDB::RowInfo();
	}

	inline FDB::RowInfo getRowByComponent(int32_t id, int32_t component_id) {
		FDB::RowTopHeader rth = Cache.getRows("ComponentsRegistry");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				FDB::RowInfo rowInfo = rth[i];
				if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == id) {
					while (rowInfo.isValid()) {
						if (*reinterpret_cast<int32_t*>(rowInfo[2].getMemoryLocation()) == component_id)
							return rth[i];
						if (rowInfo.isLinkedRowInfoValid()) {
							rowInfo = rowInfo.getLinkedRowInfo();
						}
						else {
							rowInfo = FDB::RowInfo();
						}
					}
				}
			}
			catch (...) {}
		}
		return FDB::RowInfo();
	}

	inline FDB::RowInfo FindRowByComponentID(int32_t compID, int32_t component_type) {
		FDB::RowTopHeader rth = Cache.getRows("ComponentsRegistry");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				FDB::RowInfo rowInfo = rth[i];
				while (rowInfo.isValid()) {
					if (*reinterpret_cast<int32_t*>(rowInfo[1].getMemoryLocation()) == component_type
					&&  *reinterpret_cast<int32_t*>(rowInfo[2].getMemoryLocation()) == compID)
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

	inline std::vector<std::uint32_t> GetObjectComponentTypes(int32_t lot) {
		std::vector<std::uint32_t> ret;
		FDB::RowTopHeader rth = Cache.getRows("ComponentsRegistry");
		for (int i = 0; i < rth.getRowCount(); ++i) {
			try {
				if (!rth.isValid(i)) continue;
				FDB::RowInfo rowInfo = rth[i];
				if (*reinterpret_cast<int32_t*>(rowInfo[0].getMemoryLocation()) == lot) {
					while (rowInfo.isValid()) {
						ret.push_back(*reinterpret_cast<int32_t*>(rowInfo[1].getMemoryLocation()));
						if (rowInfo.isLinkedRowInfoValid()) {
							rowInfo = rowInfo.getLinkedRowInfo();
						}
						else {
							rowInfo = FDB::RowInfo();
						}
					}
					return ret;
				}
			}
			catch (...) {}
		}
		throw new std::exception();
	}

	inline int32_t GetComponentType(int32_t id, int32_t compID) {
		return *reinterpret_cast<int32_t*>(getRowByComponent(id, compID)/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetComponentType(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetComponentID(int32_t id, int32_t compType) {
		return *reinterpret_cast<int32_t*>(getRowByComponent(id, compType)/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t GetComponentID(FDB::RowInfo row) {
		return *reinterpret_cast<int32_t*>(row/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t FindID(int32_t compID, int32_t type) {
		return *reinterpret_cast<int32_t*>(FindRowByComponentID(compID,type)/**/[0]/**/.getMemoryLocation());
	}
};

#endif // !__Cache__OBJECTS_HPP__
