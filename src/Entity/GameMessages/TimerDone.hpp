#ifndef __ENTITY__GM__TimerDone_HPP__
#define __ENTITY__GM__TimerDone_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct TimerDone : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return INTERN_TIMER_DONE;
		}

		std::u16string name;

		TimerDone() : GMBase() {}

	};
}

#endif