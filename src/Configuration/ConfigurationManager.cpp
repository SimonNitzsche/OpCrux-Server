#include "FileTypes/INIFile/ConfFile.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Configuration/ConfPorts.hpp"
#include <sys/stat.h> 

Configuration::ConfDatabase Configuration::ConfigurationManager::dbConf;
Configuration::ConfPorts Configuration::ConfigurationManager::portsConf;

void Configuration::ConfigurationManager::Load() {
	const std::string& path = "conf";
	struct stat buffer;
	if ((stat(path.c_str(), &buffer) == 0) == 0) {
		mkdirx("conf");
		dbConf.Load();
		portsConf.Load();
		Logger::log("MAIN", "Please populate the config files", LogType::UNEXPECTED);
		exit(1);
	}
	Logger::log("CONF", "Loading configuration files...");
	dbConf.Load();
	portsConf.Load();
}

void Configuration::ConfigurationManager::Reload() {
	Logger::log("CONF", "Reloading configuration files...");
	dbConf.Reload();
	portsConf.Reload();
}

void Configuration::ConfigurationManager::Save() {
	Logger::log("CONF", "Saving configuration files...");
	dbConf.Save();
	portsConf.Save();
}
