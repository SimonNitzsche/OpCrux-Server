#ifndef __FILETYPES__HKXFILE__HKENUM_HPP__
#define __FILETYPES__HKXFILE__HKENUM_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkEnum {
		std::uint32_t id;
		std::string_view name;

		hkEnum() {}
	};
}
#endif