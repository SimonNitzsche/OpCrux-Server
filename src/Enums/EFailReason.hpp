#ifndef __ENUMS__EFAILREASON_HPP__
#define __ENUMS__EFAILREASON_HPP__

#include <memory>

namespace Enums {
	enum class EFailReason : std::int32_t {
		REASON_NOT_GIVEN = 0,
		REASON_OUT_OF_IMAGINATION = 1,
		REASON_CANCELED_EARLY = 2,
		REASON_BUILD_ENDED = 3
	};
}

#endif