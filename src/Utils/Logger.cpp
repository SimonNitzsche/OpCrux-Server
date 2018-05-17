#include "Logger.hpp"
#include <string>
#include <sstream>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <fstream>
#include <ctime>
#include "ServerInfo.hpp"

void write_to_log_file(const std::string &text);
std::string timestamp_to_string(bool day, time_t timeT);
std::string ftimestamp_to_string(time_t timeT);

bool Logger::inUse = false; // Fix Multithreading

void Logger::log(std::string from, std::string message, LogType type) {
	while (inUse) {}
	inUse = true;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, (int)type);

	std::cout << "[" << from << "] " << message << std::endl;

	SetConsoleTextAttribute(hConsole, (int)15);

	write_to_log_file("[" + from + "] " + message + "\n");
	inUse = false;
}

void write_to_log_file(const std::string &text) {
	std::ofstream log_file(".//res//Logs//" + ftimestamp_to_string(ServerInfo::StartupTime) + ".txt", std::ios_base::out | std::ios_base::app);
	log_file << text;
}

std::string timestamp_to_string(bool day, time_t timeT) {
	time_t timeS = timeT;
	struct tm * timeinfo;
	char buffer[80];

	time(&timeS);
	timeinfo = localtime(&timeS);

	if (day) {
		strftime(buffer, 80, "%Y-%m-%d", timeinfo);
	}
	else {
		strftime(buffer, 80, "%I+%M+%S", timeinfo);
	}
	std::string str(buffer);

	return str;
}

std::string ftimestamp_to_string(time_t timeT) {
	return timestamp_to_string(true, timeT) + " " + timestamp_to_string(false, timeT);
}