#ifndef __ENUMS__EPROPERTYRENTALTIMEUNIT_HPP__
#define __ENUMS__EPROPERTYRENTALTIMEUNIT_HPP__

#include <memory>

namespace Enums {

	/*
		I rented this property, what's the time unit until I have to rent it again?
	*/
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
}

#endif // !__ENUMS__EPROPERTYRENTALTIMEUNIT_HPP__
