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
	static std::int64_t startupStamp();
	static std::int64_t uptime();
	static std::int64_t timestamp();
	static std::int64_t uptimeMs();
	static float uptime_normalized();
	static std::int64_t time_gone(std::int64_t uptime);
	static float CompareTimes(std::int64_t startTime, std::int64_t endTime);
	static std::string getComputerName();
	static std::string getOsName();
	static MasterServer * masterServer;
	static const std::string & GetAuthIP();
	static void SetAuthIP(const std::string & authIP);
	static std::string gameVersion;
	static void numericGameVersion(uint16_t * major, uint16_t * current, uint16_t * minor);
};
#endif // !__UTILS__SERVERINFO_HPP__