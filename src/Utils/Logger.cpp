#include "Logger.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>

#include "Common/CrossPlatform.hpp"
#include "ServerInfo.hpp"

static std::mutex LoggerMutex;

static std::string stringifyTimestamp(std::time_t t, const char *format)
{
	const std::tm *timeinfo = std::localtime(&t);
	
	char buffer[256];
	auto len = std::strftime(buffer, 256, format, timeinfo);
	buffer[len] = '\0'; //string is nullterminated even when strftime fails
	
	return buffer;
}

static std::string formatTimestamp(std::time_t t)
{
	return stringifyTimestamp(t, "%Y-%m-%d %H-%M-%S");
}

static void writeLogfile(const std::string &text)
{
	std::ofstream log_file("./logs/" + formatTimestamp(ServerInfo::StartupTime) + ".txt", std::ios::out | std::ios::app);
	log_file << text;
}

void Logger::log(const std::string &from, const std::string &message, LogType type) {
	
	std::lock_guard<std::mutex> lock(LoggerMutex);

#ifdef OPCRUX_PLATFORM_WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, static_cast<int>(type));

	std::cout << "[" << from << "] " << message << std::endl;

	SetConsoleTextAttribute(hConsole, 15);
#else
	std::cout << "\033[" << static_cast<int>(type) << "m[" << from << "] " << message << "\033[0m" << std::endl;
#endif

	writeLogfile("[" + from + "] " + message + "\n");
}
