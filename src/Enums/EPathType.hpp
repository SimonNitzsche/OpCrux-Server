#ifndef __ENUMS__EPATHTYPE_HPP__
#define __ENUMS__EPATHTYPE_HPP__
#include <memory>

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

#endif