#ifndef __FILETYPES__HKXFILE__HKTYPE_HPP__
#define __FILETYPES__HKXFILE__HKTYPE_HPP__
#include <memory>
#include <string>
#include <vector>

#include "FileTypes/HKXFile/hkEnum.hpp"
#include "FileTypes/HKXFile/hkTypeMember.hpp"

namespace HKX {
	struct hkType {
		std::uint32_t object_size;
		std::string name;
		std::string class_name;
		std::uint32_t described_version;
		std::uint32_t num_implemented_interfaces;
		std::uint32_t declared_enums;
		std::uint32_t address;

		std::vector<hkTypeMember> members;
		std::vector<hkEnum> enums;
		std::vector<std::vector<hkEnum>> sub_enums;
		std::vector<std::string_view> sub_enum_names;

		hkType* parent;
		std::uint32_t parent_address;

		hkType() {}
		void reset() {
			members.clear();
			enums.clear();
			sub_enums.clear();
			sub_enum_names.clear();
			name = "";

			parent = nullptr;
			parent_address = 0;
		}
	};
}
#endif