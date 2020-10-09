#ifndef __DATATYPES__ZONEINFO_HPP__
#define __DATATYPES__ZONEINFO_HPP__

#include <memory>

namespace DataTypes {

	/*
		Contains all Info we need about a Zone.
	*/
	struct ZoneInfo {
		uint16_t zoneID;
		uint16_t zoneInstance;
		uint32_t zoneClone;

		ZoneInfo() : zoneID(0), zoneInstance(0), zoneClone(0) {}
	};
}

#endif