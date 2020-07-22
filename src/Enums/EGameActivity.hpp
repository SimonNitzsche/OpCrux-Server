#ifndef __ENUMS__EGAMEACTIVITY_HPP__
#define __ENUMS__EGAMEACTIVITY_HPP__

#include <memory>

namespace Enums {
	enum class EGameActivity : std::uint32_t {
		NONE = 0,
		QUICKBUILD = 1,
		SHOOTING_GALLERY = 2,
		RACING = 3,
		PINBALL = 4,
		PET_TAMING = 5
	};
}
#endif // !__ENUMS__EGAMEACTIVITY_HPP__