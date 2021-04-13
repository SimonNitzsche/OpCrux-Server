#ifndef __ENTITY__GM__StopPathing_HPP__
#define __ENTITY__GM__StopPathing_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct StopPathing : public GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::INTERN_STOP_PATHING;
		}

		StopPathing() : GMBase() {}
	};
}

#endif