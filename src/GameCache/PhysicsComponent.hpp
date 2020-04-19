#ifndef __GAMECACHE__PHYSICSCOMPONENT_HPP__
#define __GAMECACHE__PHYSICSCOMPONENT_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CachePhysicsComponent {
	inline FDB::RowInfo getRow(int32_t id) {
		FDB::RowTopHeader rth = Cache.getRows("PhysicsComponent");
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

	inline float GetStatic(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[1]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetPhysicsAsset(int32_t id) {
		return getRow(id)/**/[2]/**/;
	}

	inline float GetJump(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[3]/**/.getMemoryLocation());
	}

	inline float GetDoubleJump(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[4]/**/.getMemoryLocation());
	}

	inline float GetSpeed(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[5]/**/.getMemoryLocation());
	}

	inline float GetRotSpeed(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[6]/**/.getMemoryLocation());
	}

	inline float GetPlayerHeight(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[7]/**/.getMemoryLocation());
	}

	inline float GetPlayerRadius(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[8]/**/.getMemoryLocation());
	}

	inline int32_t GetPcShapeType(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[9]/**/.getMemoryLocation());
	}

	inline int32_t GetCollisionGroup(int32_t id) {
		return *reinterpret_cast<int32_t*>(getRow(id)/**/[10]/**/.getMemoryLocation());
	}

	inline float GetAirSpeed(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[11]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetBoundaryAsset(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[12]/**/.getMemoryLocation());
	}

	inline float GetJumpAirSpeed(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[13]/**/.getMemoryLocation());
	}

	inline float GetFriction(int32_t id) {
		return *reinterpret_cast<float*>(getRow(id)/**/[14]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetGravityVolumeAsset(int32_t id) {
		return FDB::PointerString(&Cache, getRow(id)/**/[15]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__PHYSICSCOMPONENT_HPP__
