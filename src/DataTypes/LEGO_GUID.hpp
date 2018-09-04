#ifndef __DATATYPES__LEGO_GUID_HPP__
#define __DATATYPES__LEGO_GUID_HPP__
#include <stdlib.h>
#include <stdint.h>
#include <memory>

typedef struct LEGO_GUID {
	uint32_t Data1;
	uint16_t Data2;
	uint16_t Data3;
	uint64_t Data4;
};

#endif // !__DATATYPES__LEGO_GUID_HPP__
