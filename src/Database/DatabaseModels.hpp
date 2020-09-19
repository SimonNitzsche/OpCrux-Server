#ifndef __DATABASE__DATABASEMODELS_HPP__
#define __DATABASE__DATABASEMODELS_HPP__

namespace DatabaseModels {
	struct Str_DB_CharStyle {
public:
	std::int16_t headColor;
	std::int16_t head;
	std::int16_t chestColor;
	std::int16_t chest;
	std::int16_t legs;
	std::int16_t hairStyle;
	std::int16_t hairColor;
	std::int16_t leftHand;
	std::int16_t rightHand;
	std::int16_t eyebrowStyle;
	std::int16_t eyesStyle;
	std::int16_t mouthStyle;
};

struct Str_DB_CharStats {
public:
	std::uint64_t charIndex;
	std::uint32_t TotalCurrencyCollected;
	std::uint32_t TotalBricksCollected;
	std::uint32_t TotalSmashablesSmashed;
	std::uint32_t TotalQuickBuildsCompleted;
	std::uint32_t TotalEnemiesSmashed;
	std::uint32_t TotalRocketsUsed;
	std::uint32_t TotalMissionsCompleted;
	std::uint32_t TotalPetsTamed;
	std::uint32_t TotalImaginationPowerUpsCollected;
	std::uint32_t TotalLifePowerUpsCollected;
	std::uint32_t TotalArmorPowerUpsCollected;
	std::uint32_t TotalDistanceTraveled;
	std::uint32_t TotalSuicides;
	std::uint32_t TotalDamageTaken;
	std::uint32_t TotalDamageHealed;
	std::uint32_t TotalArmorRepaired;
	std::uint32_t TotalImaginationRestored;
	std::uint32_t TotalImaginationUsed;
	std::uint32_t TotalDistanceDriven;
	std::uint32_t TotalTimeAirborne;
	std::uint32_t TotalRacingImaginationPowerUpsCollected;
	std::uint32_t TotalRacingImaginationCratesSmashed;
	std::uint32_t TotalRacecarBoostsActivated;
	std::uint32_t TotalRacecarWrecks;
	std::uint32_t TotalRacingSmashablesSmashed;
	std::uint32_t TotalRacesFinished;
	std::uint32_t TotalFirstPlaceFinishes;
};

enum class EStats {
	TotalCurrencyCollected = 1,
	TotalBricksCollected,
	TotalSmashablesSmashed,
	TotalQuickBuildsCompleted,
	TotalEnemiesSmashed,
	TotalRocketsUsed,
	TotalMissionsCompleted,
	TotalPetsTamed,
	TotalImaginationPowerUpsCollected,
	TotalLifePowerUpsCollected,
	TotalArmorPowerUpsCollected,
	TotalDistanceTraveled,
	TotalSuicides,
	TotalDamageTaken,
	TotalDamageHealed,
	TotalArmorRepaired,
	TotalImaginationRestored,
	TotalImaginationUsed,
	TotalDistanceDriven,
	TotalTimeAirborne,
	TotalRacingImaginationPowerUpsCollected,
	TotalRacingImaginationCratesSmashed,
	TotalRacecarBoostsActivated,
	TotalRacecarWrecks,
	TotalRacingSmashablesSmashed,
	TotalRacesFinished,
	TotalFirstPlaceFinishes
};

struct Str_DB_CharInfo {
public: 
	std::uint32_t accountID;
	std::uint64_t objectID;
	std::uint8_t charIndex;
	std::string name = "";
	std::string pendingName = "";
	std::uint16_t styleID;
	std::uint16_t statsID;
	std::uint16_t lastWorld;
	std::uint16_t lastInstance;
    std::uint16_t lastClone;
	std::uint64_t lastLog;
	DataTypes::Vector3 position;
	std::uint16_t shirtObjectID;
	std::uint16_t pantsObjectID;
	std::uint16_t uScore;
	std::uint16_t uLevel;
	std::uint16_t currency;
	std::uint16_t reputation;
	std::uint16_t health;
	std::uint16_t imagination;
	std::uint16_t armor;
	Str_DB_CharInfo() {
		name = "";
		pendingName = "";
	}
};

struct MissionModel {
	std::int64_t charID;
	std::int32_t missionID;
	std::int32_t state;
	std::string progress;
	std::int32_t repeatCount;
	std::int64_t time;
	std::int32_t chosenReward;

	void updateTime() {
		time = ::time(0);
	}

};

struct ItemModel {
	std::uint64_t objectID;
	std::uint64_t ownerID;
	std::uint64_t subkey;
	std::uint32_t tab;
	std::uint32_t slot;
	std::uint32_t templateID;
	std::uint32_t count;
	struct ItemAttributes {
		/*
			0x01 = bound
			0x02 = equipped
		*/
	private:
		std::uint16_t data;
	public:
		ItemAttributes() {
			data = 0;
		}

		inline std::uint16_t GetAttributes() {
			return data;
		}

		inline void SetAttributes(std::uint16_t attributes) {
			data = attributes;
		}

		inline bool GetAttribute(std::uint16_t index) {
			if (index > 0x08) throw std::out_of_range("Index higher than 8");
			return data & (std::uint16_t(1) << index);
		}

		inline void SetAttribute(std::uint16_t index, bool flag) {
			if (index > 0x08) throw std::out_of_range("Index higher than 8");
			data ^= (-flag ^ data) & (std::uint16_t(1) << index);
		}

		inline bool GetBound() {
			return GetAttribute(0x00);
		}

		inline void SetBound(bool flag) {
			SetAttribute(0x00, flag);
		}

		inline bool GetEquipped() {
			return GetAttribute(0x01);
		}

		inline void SetEquipped(bool flag) {
			SetAttribute(0x01, flag);
		}
	} attributes;
	LDFCollection metadata;

	ItemModel() {
		subkey = 0;
		count = 1;
	}
};
}

#endif