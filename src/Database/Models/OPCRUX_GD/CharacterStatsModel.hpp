#ifndef __DB_MODELS_GD__CHARACTERSTATS
#define __DB_MODELS_GD__CHARACTERSTATS

#include "Database/Models/DBModel.hpp"
namespace Database {
	namespace OPCRUX_GD {
		struct CharacterStatsModel : public DBModel {
			public static const char* DB_CATEGORY = "GD";
			public static const char* DB_TABLE = "CharacterStats";

			public static const char*[] FIELDS = {
				__DB_MODELS_GD__CHARACTERSTATS_FIELDS((x,y)[] = > {return x; })
			};

			#define __DB_MODELS_GD__CHARACTERSTATS_FIELDS(func) {\
				func("statsID", std::uint64_t);\
				func("currencyCollected", std::uint64_t);\
				func("bricksCollected", std::uint64_t);\
				func("objectsSmashed", std::uint64_t);\
				func("quickbuildsCompleted", std::uint64_t);\
				func("enemiesSmashed", std::uint64_t);\
				func("rocketsUsed", std::uint64_t);\
				func("missionsCompleted", std::uint64_t);\
				func("petsTamed", std::uint64_t);\
				func("imaginationCollected", std::uint64_t);\
				func("healthCollected", std::uint64_t);\
				func("armorCollected", std::uint64_t);\
				func("distanceTraveled", std::uint64_t);\
				func("smashed", std::uint64_t);\
				func("damageTaken", std::uint64_t);\
				func("armorRepaired", std::uint64_t);\
				func("imaginationRestored", std::uint64_t);\
				func("imaginationUsed", std::uint64_t);\
				func("distanceDriven", std::uint64_t);\
				func("airborneTime", std::uint64_t);\
				func("racingImagintaionCollected", std::uint64_t);\
				func("racingImaginationCratesSmashed", std::uint64_t);\
				func("racingBoostsActivated", std::uint64_t);\
				func("racingWrecks", std::uint64_t);\
				func("racingSmashablesSmashed", std::uint64_t);\
				func("racesFinished", std::uint64_t);\
				func("racesWon", std::uint64_t);\
			}

		};
	}
}

#endif