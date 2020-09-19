#ifndef __DATATYPES__LEGO_GUID_HPP__
#define __DATATYPES__LEGO_GUID_HPP__
#include <stdlib.h>
#include <stdint.h>
#include <memory>

namespace DataTypes {

	/*
		Contitutes a LEGO Global Unique Identifier.
	*/
	typedef struct LEGO_GUID {
		std::uint32_t Data1;
		std::uint16_t Data2;
		std::uint16_t Data3;
		std::uint64_t Data4;
	};
}

#endif // !__DATATYPES__LEGO_GUID_HPP__
