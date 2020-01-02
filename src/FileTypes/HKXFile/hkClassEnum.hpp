#ifndef __FILETYPES__HKXFILE__HKCLASSENUM_HPP__
#define __FILETYPES__HKXFILE__HKCLASSENUM_HPP__
	
#include <memory>
#include <string>
#include <map>

namespace HKX {
	class hkClassEnum {
		std::string_view name;
		std::map<std::uint32_t, std::string_view> items;

		hkClassEnum() {
			
		}
	};
}

#endif