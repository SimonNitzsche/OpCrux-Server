#ifndef __ENUMS__EPROPERTYRENTALTIMEUNIT_HPP__
#define __ENUMS__EPROPERTYRENTALTIMEUNIT_HPP__

#include <memory>

enum class EPropertyRentalTimeUnit : int32_t {
	Forever = 0,
	Seconds,
	Minutes,
	Hours,
	Days,
	Weeks,
	Months,
	Years
};

#endif