#ifndef __GAMECACHE__SKILLBEHAVIOR_HPP__
#define __GAMECACHE__SKILLBEHAVIOR_HPP__

#include "Interface/FastDatabase.hpp"
#include "Utils/Logger.hpp"
using namespace GameCache::Interface;
extern FDB::Connection Cache;

namespace CacheSkillBehavior {
	inline FDB::RowInfo getRow(int skillID) {
		FDB::RowTopHeader rth = Cache.getRows("SkillBehavior");
		for(int  i = 0; i < rth.getRowCount(); ++i) {
			if (!rth.isValid(i)) continue;
			try {
				if (*reinterpret_cast<int32_t*>(rth[i][0].getMemoryLocation()) == skillID)
					return rth[i];
			}
			catch (std::runtime_error e) {
				Logger::log("Cache:SkillBehavior", e.what(), LogType::ERR);
			}
		}
		return FDB::RowInfo();
	}

	inline int32_t GetLocaleStatus(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[1]/**/.getMemoryLocation());
	}

	inline int32_t GetBehaviorID(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[2]/**/.getMemoryLocation());
	}

	inline int32_t GetImaginationCost(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[3]/**/.getMemoryLocation());
	}

	inline int32_t GetCooldownGroup(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[4]/**/.getMemoryLocation());
	}

	inline float GetCooldown(int32_t skillID) {
		return *reinterpret_cast<float*>(getRow(skillID)/**/[5]/**/.getMemoryLocation());
	}

	inline bool GetIsNpcEditor(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[6]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString GetOomSkillID(int32_t skillID) {
		return FDB::PointerString(&Cache, getRow(skillID)/**/[7]/**/.getMemoryLocation());
	}

	inline int32_t GetOomBehaviorEffectID(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[8]/**/.getMemoryLocation());
	}

	inline int32_t GetCastTypeDesc(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[9]/**/.getMemoryLocation());
	}

	inline int32_t GetImaginationBonusUI(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[10]/**/.getMemoryLocation());
	}

	inline int32_t GetLifeBonusUI(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[11]/**/.getMemoryLocation());
	}

	inline int32_t GetArmorBonusUI(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[12]/**/.getMemoryLocation());
	}

	inline int32_t GetDamageUI(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[13]/**/.getMemoryLocation());
	}

	inline bool GetHideIcon(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[14]/**/.getMemoryLocation()) == 1;
	}

	inline bool GetLocalize(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[15]/**/.getMemoryLocation()) == 1;
	}

	inline FDB::PointerString GetGateVersion(int32_t skillID) {
		return FDB::PointerString(&Cache, getRow(skillID)/**/[16]/**/.getMemoryLocation());
	}

	inline int32_t GetCancelType(int32_t skillID) {
		return *reinterpret_cast<int32_t*>(getRow(skillID)/**/[17]/**/.getMemoryLocation());
	}
};

#endif // !__GAMECACHE__SKILLBEHAVIOR_HPP__
