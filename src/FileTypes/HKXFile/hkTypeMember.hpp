#ifndef __FILETYPES__HKXFILE__HKTYPEMEMBER_HPP__
#define __FILETYPES__HKXFILE__HKTYPEMEMBER_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkTypeMember {
		std::uint8_t tag[2];
		std::uint16_t array_size;
		std::uint16_t struct_type;
		std::uint16_t offset;
		std::uint32_t structure_address;
		std::string_view name;
		std::string_view structure;

		hkTypeMember() {}
	};
}
#endif