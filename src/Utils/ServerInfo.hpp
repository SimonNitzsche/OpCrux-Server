#ifndef __UTILS__SERVERINFO_HPP__
#define __UTILS__SERVERINFO_HPP__

#include <stdio.h>
#include <string>
#include <time.h>
#include <stdint.h>
#include "Common/CrossPlatform.hpp"

class MasterServer;

class ServerInfo {
private:
	static clock_t appstart;
	static bool initDone;
	static std::string m_authIP;
public:
	static time_t StartupTime;
	static int processID;
	static bool bRunning;
	static const char* baseTitle;
	static void init();
	static long long startupStamp();
	static long long uptime();
	static long long timestamp();
	static long long uptimeMs();
	static float uptime_normalized();
	static long long time_gone(long long uptime);
	static float CompareTimes(long long startTime, long long endTime);
	static std::string getComputerName();
	static std::string getOsName();
	static MasterServer * masterServer;
	static const std::string & GetAuthIP();
	static void SetAuthIP(const std::string & authIP);
	static std::string gameVersion;
	static void numericGameVersion(uint16_t * major, uint16_t * current, uint16_t * minor);
	static std::uint64_t WindowsGetSystemTimeAsUnixTime();
	static std::uint64_t GetSystemTimeAsUnixTime();
};
#endif // !__UTILS__SERVERINFO_HPP__