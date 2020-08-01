#ifndef __ENUMS__EFRIENDREQUESTCODES_HPP__
#define __ENUMS__EFRIENDREQUESTCODES_HPP__

#include <memory>

namespace Enums {
	enum class EFriendRequestCodes : std::int8_t {
		ACCEPTED = 0,
		DECLINED = 1,
		WINDOW_CLOSED = 3
	};
}

#endif