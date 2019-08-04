#ifndef __UTILS__LOGGER_HPP__
#define __UTILS__LOGGER_HPP__

#include <string>

#include "Common/Platformident.hpp"

//
// global logging
//

enum LogType
{
#ifdef OPCRUX_PLATFORM_WIN32
	NORMAL = 15,
	INFO = 11,
	WARN = 14,
	ERR = 12,
	PASSED = 10,
	UNEXPECTED = 13
#else
	NORMAL = 37,
	INFO = 36,
	WARN = 33,
	ERR = 31,
	PASSED = 32,
	UNEXPECTED = 35
#endif

};

namespace Logger
{
	void log(const std::string &from, const std::string &message, LogType type = NORMAL);
}

// Displays a variable's name and value
#define varinfo(x) std::cout << #x": " << x << "\n";

#endif // !__UTILS__LOGGER_HPP__
