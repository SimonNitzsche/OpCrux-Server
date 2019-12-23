#ifndef __ENTITY__GM__RestoreToPostLoadStats_HPP__
#define __ENTITY__GM__RestoreToPostLoadStats_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct RestoreToPostLoadStats : GMBase {
		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_RESTORE_TO_POST_LOAD_STATS;
		}

		RestoreToPostLoadStats() {}
	};
}

#endif