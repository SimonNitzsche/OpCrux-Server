#ifndef __ENUMS__EPATHBEHAVIOR_HPP__
#define __ENUMS__EPATHBEHAVIOR_HPP__

#include <memory>

namespace Enums {

	/*
		Literally path behavior.
	*/
	enum class EPathBehavior : uint32_t {
		Loop = 0,
		Bounce,
		Once
	};
}

#endif // !__ENUMS__EPATHBEHAVIOR_HPP__
