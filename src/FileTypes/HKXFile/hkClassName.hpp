#ifndef __FILETYPES__HKXFILE__HKCLASSNAME_HPP__
#define __FILETYPES__HKXFILE__HKCLASSNAME_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkClassName {
		std::uint32_t tag;
		std::string_view name;

		hkClassName() {}
	};
}
#endif