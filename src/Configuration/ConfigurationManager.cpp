#include "FileTypes/INIFile/ConfFile.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfigurationManager.hpp"
#include "Configuration/ConfPorts.hpp"

Configuration::ConfDatabase Configuration::ConfigurationManager::dbConf;
Configuration::ConfPorts Configuration::ConfigurationManager::portsConf;

void Configuration::ConfigurationManager::Load() {
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