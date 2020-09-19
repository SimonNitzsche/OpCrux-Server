#ifndef __DATATYPES__ZONEINFO_HPP__
#define __DATATYPES__ZONEINFO_HPP__

#include <memory>

namespace DataTypes {

	/*
		Contains all Info we need about a Zone.
	*/
	struct ZoneInfo {
		std::uint16_t zoneID;
        std::uint16_t zoneInstance;
        std::uint32_t zoneClone;
	};
}

#endif