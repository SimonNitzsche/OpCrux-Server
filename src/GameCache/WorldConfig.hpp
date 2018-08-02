#ifndef __GAMECACHE__WORLDCONFIG_HPP__
#define __GAMECACHE__WORLDCONFIG_HPP__
#include "GameCache/Interface/FastDatabase.hpp"
extern FDB::Connection GameCache;

namespace CacheWorldConfig {
	inline float GetPEGravityValue() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[1]/**/.getMemoryLocation());
	}

	inline float GetPEBroadphaseWorldSize() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[2]/**/.getMemoryLocation());
	}

	inline float GetPEGameObjScaleFactor() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[3]/**/.getMemoryLocation());
	}

	inline float GetCharacterRotationSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[4]/**/.getMemoryLocation());
	}

	inline float GetCharacterWalkForwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[5]/**/.getMemoryLocation());
	}

	inline float GetCharacterWalkBackwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[6]/**/.getMemoryLocation());
	}

	inline float GetCharacterWalkStrafeSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[7]/**/.getMemoryLocation());
	}

	inline float GetCharacterWalkStrafeForwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[8]/**/.getMemoryLocation());
	}

	inline float GetCharacterWalkStrafeBackwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[9]/**/.getMemoryLocation());
	}

	inline float GetCharacterRunBackwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[10]/**/.getMemoryLocation());
	}

	inline float GetCharacterRunStrafeSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[11]/**/.getMemoryLocation());
	}

	inline float GetCharacterRunStrafeForwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[12]/**/.getMemoryLocation());
	}

	inline float GetCharacterRunStrafeBackwardSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[13]/**/.getMemoryLocation());
	}

	inline float GetGlobalCooldown() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[14]/**/.getMemoryLocation());
	}

	inline float GetCharacterGroundedTime() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[15]/**/.getMemoryLocation());
	}

	inline float GetCharacterGroundedSpeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[16]/**/.getMemoryLocation());
	}

	inline float GetGlobalImmunityTime() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[17]/**/.getMemoryLocation());
	}

	inline float GetCharacterMaxSlope() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[18]/**/.getMemoryLocation());
	}

	inline float GetDefaultRespawnTime() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[19]/**/.getMemoryLocation());
	}

	inline float GetMissionTooltipTimeout() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[20]/**/.getMemoryLocation());
	}

	inline float GetVendorBuyMultiplier() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[21]/**/.getMemoryLocation());
	}

	inline float GetPetFollowRadius() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[22]/**/.getMemoryLocation());
	}

	inline float GetCharacterEyeHeight() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[23]/**/.getMemoryLocation());
	}

	inline float GetFlightVerticalVelocity() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[24]/**/.getMemoryLocation());
	}

	inline float GetFlightAirspeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[25]/**/.getMemoryLocation());
	}

	inline float GetFlightFuelRatio() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[26]/**/.getMemoryLocation());
	}

	inline float GetFlightMaxAirspeed() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[27]/**/.getMemoryLocation());
	}

	inline float GetReputationPerVote() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[28]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyCloneLimit() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[29]/**/.getMemoryLocation());
	}

	inline int32_t GetDefaultHomespaceTemplate() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[30]/**/.getMemoryLocation());
	}

	inline float GetCoinsLostOnDeathPercentage() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[31]/**/.getMemoryLocation());
	}

	inline int32_t GetCoinsLostOnDeathMin() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[32]/**/.getMemoryLocation());
	}

	inline int32_t GetCoinsLostOnDeathMax() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[33]/**/.getMemoryLocation());
	}

	inline int32_t GetCharacterVotesPerDay() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[34]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModerationRequestApprovalCost() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[35]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModerationRequestReviewCost() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[36]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModRequestAllowedSpike() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[37]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModRequestAllowedInterval() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[38]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModRequestsAllowedTotal() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[39]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModRequestsSpikeDuration() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[40]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyModRequestsIntervalDuration() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[41]/**/.getMemoryLocation());
	}

	inline bool GetModelModerateOnCreate() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[42]/**/.getMemoryLocation())!=0;
	}

	inline float GetDefaultPropertyMaxHeight() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[43]/**/.getMemoryLocation());
	}

	inline float GetReputationPerVoteCast() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[44]/**/.getMemoryLocation());
	}

	inline float GetReputationPerVoteReceived() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[45]/**/.getMemoryLocation());
	}

	inline int32_t GetShowcaseTopModelConsiderationBattles() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[46]/**/.getMemoryLocation());
	}

	inline float GetReputationPerBattlePromotion() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[47]/**/.getMemoryLocation());
	}

	inline float GetCoinsLostOnDeathMinTimeout() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[48]/**/.getMemoryLocation());
	}

	inline float GetCoinsLostOnDeathMaxTimeout() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[49]/**/.getMemoryLocation());
	}

	inline int32_t GetMailBaseFee() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[50]/**/.getMemoryLocation());
	}

	inline float GetMailPercentAttachmentFee() {
		return *reinterpret_cast<float*>(GameCache.getRows("WorldConfig")[0]/**/[51]/**/.getMemoryLocation());
	}

	inline int32_t GetPropertyReputationDelay() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[52]/**/.getMemoryLocation());
	}

	inline int32_t GetLevelCap() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[53]/**/.getMemoryLocation());
	}

	inline FDB::PointerString GetLevelUpBehaviorEffect() {
		return FDB::PointerString(&GameCache, GameCache.getRows("WorldConfig")[0]/**/[54]/**/.getMemoryLocation());
	}

	inline int32_t GetCharacterVersion() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[55]/**/.getMemoryLocation());
	}

	inline int32_t GetLevelCapCurrencyConversion() {
		return *reinterpret_cast<int32_t*>(GameCache.getRows("WorldConfig")[0]/**/[56]/**/.getMemoryLocation());
	}
}

#endif