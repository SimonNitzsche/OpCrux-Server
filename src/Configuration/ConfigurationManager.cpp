#include "FileTypes/INIFile/ConfFile.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfGeneral.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Configuration/ConfPorts.hpp"
#include <sys/stat.h> 

Configuration::ConfDatabase Configuration::ConfigurationManager::dbConf;
Configuration::ConfPorts Configuration::ConfigurationManager::portsConf;
Configuration::ConfGeneral Configuration::ConfigurationManager::generalConf;

void Configuration::ConfigurationManager::Load() {
	const std::string& path = "conf";
	struct stat buffer;
	if ((stat(path.c_str(), &buffer) == 0) == 0) {
		mkdirx("conf");
		dbConf.Load();
		portsConf.Load();
		generalConf.Load();
		Logger::log("MAIN", "Please populate the config files", LogType::UNEXPECTED);
		exit(1);
	}
	Logger::log("CONF", "Loading configuration files...");
	dbConf.Load();
	portsConf.Load();
	generalConf.Load();
}

void Configuration::ConfigurationManager::Reload() {
	Logger::log("CONF", "Reloading configuration files...");
	dbConf.Reload();
	portsConf.Reload();
	generalConf.Reload();
}

void Configuration::ConfigurationManager::Save() {
	Logger::log("CONF", "Saving configuration files...");
	dbConf.Save();
	portsConf.Save();
	generalConf.Save();
}
