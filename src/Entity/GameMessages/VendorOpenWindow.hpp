#ifndef __ENTITY__GM__VendorOpenWindow_HPP__
#define __ENTITY__GM__VendorOpenWindow_HPP__
#include "Entity/GameMessages.hpp"

namespace GM {
	struct VendorOpenWindow : GMBase {

		inline constexpr static Enums::EGameMessageID GetID() {
			return Enums::EGameMessageID::SERVER_OPEN_VENDOR_WINDOW;
		}
	};
}

#endif