#include "ServerInfo.hpp"

#include "Common/CrossPlatform.hpp"

clock_t ServerInfo::appstart;
bool ServerInfo::initDone = false;
bool ServerInfo::bRunning = true;
const char* ServerInfo::baseTitle = "LUR Server 3.0";
time_t ServerInfo::StartupTime;

int ServerInfo::processID;

void ServerInfo::init() {
	if (!initDone) {
		initDone = true; //Should be up there

#ifdef WIN32
		processID = (int)((unsigned int)GetCurrentProcessId());
#else
		processID = (int)getpid();
#endif
appstart = clock();
		StartupTime = time(0);
	}
}

long long ServerInfo::uptime() {
	return (clock() / CLOCKS_PER_SEC) - appstart;
}

float ServerInfo::uptime_normalized() {
	return ServerInfo::uptime() * 0.001f;
}

long long ServerInfo::time_gone(long long uptime) {
	return uptime - appstart;
}

float ServerInfo::CompareTimes(long long startTime, long long endTime) {
	return (endTime - startTime) * 0.001f;
}

std::string ServerInfo::getComputerName() {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	return getenv("COMPUTERNAME");
#endif // WINDOWS
#if defined(LINUX) || defined(MAC) || defined(__CYGWIN__)
	return getenv("HOSTNAME");
#endif // LINUX || MAC
	return ("Function not supported on this operating system.");
}

std::string ServerInfo::getOsName() {
#if defined(_WIN32) || defined(_WIN64)
	return "Windows";
#elif defined(__unix) || defined(__unix__)
	return "Unix";
#elif defined(__APPLE__) || defined(__MACH__)
	return "Mac OSX";
#elif defined(__linux__)
	return "Linux";
#elif defined(__FreeBSD__)
	return "FreeBSD";
#else
	return "Other";
#endif
}