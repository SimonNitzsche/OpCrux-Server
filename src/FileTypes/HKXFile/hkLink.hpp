#ifndef __FILETYPES__HKXFILE__HKLINK_HPP__
#define __FILETYPES__HKXFILE__HKLINK_HPP__
#include <memory>

#include "FileTypes/HKXFile/hkType.hpp"

namespace HKX {
	struct hkLink {
		std::uint32_t type;
		std::uint32_t address_1;
		std::uint32_t address_2;

		hkType* type_parent;
		hkType* type_node;

		hkLink() {}
	};
}
#endif