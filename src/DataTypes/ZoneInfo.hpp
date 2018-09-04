#ifndef __DATATYPES__ZONEINFO_HPP__
#define __DATATYPES__ZONEINFO_HPP__

#include <memory>

struct ZoneInfo {
	uint16_t zoneID;
	uint16_t zoneInstance;
	uint32_t zoneClone;
};

#endif