#ifndef __ENUMS__ETERMINATETYPE_HPP__
#define __ENUMS__ETERMINATETYPE_HPP__

#include <memory>

namespace Enums {
	enum class ETerminateType : std::int32_t {
		RANGE = 0,
		USER = 1,
		FROM_INTERACTION = 2
	};
}

#endif