#pragma once
#ifndef UTILS_LOGGER_HPP
#define UTILS_LOGGER_HPP
#include <string>

enum LogType {
#ifdef __unix__
	NORMAL = 37,
	INFO = 36,
	WARN = 33,
	ERR = 31,
	PASSED = 32,
	UNEXPECTED = 35
#endif
#ifdef WIN32
	NORMAL = 15,
	INFO = 11,
	WARN = 0xe,
	ERR = 12,
	PASSED = 10,
	UNEXPECTED = 13
#endif
};

class Logger {
private:
	static bool inUse;
public:
	static void log(std::string from, std::string message, LogType type = NORMAL);
};

// Displays a variable's name and value
#define varinfo(x) std::cout << #x": " << x << "\n";
#endif
