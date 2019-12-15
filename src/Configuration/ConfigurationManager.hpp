#ifndef __CONFIGURATION__CONFIGURATIONMANAGER_HPP__
#define __CONFIGURATION__CONFIGURATIONMANAGER_HPP__

#include "FileTypes/INIFile/ConfFile.hpp"
#include "Configuration/ConfDatabase.hpp"

namespace Configuration {
	class ConfigurationManager {
	public:
		static ConfDatabase dbConf;

		static void Load();

		static void Reload();

		static void Save();
	};
}

#endif