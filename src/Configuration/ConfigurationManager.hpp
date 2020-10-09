#ifndef __CONFIGURATION__CONFIGURATIONMANAGER_HPP__
#define __CONFIGURATION__CONFIGURATIONMANAGER_HPP__

#include "FileTypes/INIFile/ConfFile.hpp"
#include "Configuration/ConfDatabase.hpp"
#include "Configuration/ConfGeneral.hpp"
#include "Configuration/ConfPorts.hpp"

namespace Configuration {
	class ConfigurationManager {
	public:
		static ConfDatabase dbConf;
		static ConfPorts portsConf;
		static ConfGeneral generalConf;

		static void Load();

		static void Reload();

		static void Save();
	};
}

#endif