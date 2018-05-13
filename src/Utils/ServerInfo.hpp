#pragma once
#ifndef UTILS_SERVERINFO_HPP

#include <stdio.h>
#include <string>
#ifdef _LINUX || MAC
#include <unistd.h>
#endif
#ifdef WIN32
#include <windows.h>
#endif
#include <time.h>

typedef long clock_t;

class ServerInfo {
private:
	static clock_t appstart;
	static bool initDone;
public:
	static time_t StartupTime;
	static int processID;
	static bool bRunning;
	static const char* baseTitle;
	static void init();
	static long long uptime();
	static float uptime_normalized();
	static long long time_gone(long long uptime);
	static float CompareTimes(long long startTime, long long endTime);
	static std::string getDatabaseConfig(std::string key, std::string defaultVal);
	static std::string getComputerName();
	static std::string getOsName();
};
#endif