#ifndef __ENUMS__EPATHTYPE_HPP__
#define __ENUMS__EPATHTYPE_HPP__

#include <memory>

namespace Enums {

	/*
		Literally the Path Type.
	*/
	enum class EPathType : uint32_t {
		Movement = 0,
		MovingPlatform,
		Property,
		Camera,
		Spawner,
		Showcase,
		Race,
		Rail
	};
}

#endif // !__ENUMS__EPATHTYPE_HPP__
