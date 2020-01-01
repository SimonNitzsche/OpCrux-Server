#ifndef __FILETYPES__HKXFILE__HKPOINTER_HPP__
#define __FILETYPES__HKXFILE__HKPOINTER_HPP__
#include <memory>

namespace HKX {
	struct hkPointer {
		std::uint32_t abs_address;
		std::uint32_t target_address;

		hkPointer() {}
	};
}
#endif