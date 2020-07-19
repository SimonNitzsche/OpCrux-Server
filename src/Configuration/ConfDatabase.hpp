#ifndef __CONFIGURATION__CONFDATABASE_HPP__
#define __CONFIGURATION__CONFDATABASE_HPP__

#include "FileTypes/INIFile/ConfFile.hpp"

namespace Configuration {
	class ConfDatabase : public ConfFile {
	private:
		std::string GetFilename() const { return "Database.ini"; }
		std::string GetComment() const { return \
			";===========================\n"\
			";Database Configuration File\n"\
			";===========================\n";
		}
		ConfFileStruct GetConfTemplate() const { return {
			// [DBConnection]
			{"DBConnection", {
				// DBDRIVER
				{"DBDRIVER", "<Database Driver>"},
				// DBHOST
				{"DBHOST", "<Database Host>"},
				// DBUSER
				{"DBUSER", "<Database User>"},
				// DBPASS
				{"DBPASS", "<Database Password>"}
			}},

			// [DBGameDB]
			{"DBGameDB", {
				// DBNAME
				{"DBNAME", "OPCRUX_GD"}
			}},

			// [DBCounterDB]
			{"DBCounterDB", {
				// DBNAME
				{"DBNAME", "OPCRUX_CD"}
			}},

			// [DBAccountDB]
			{"DBAccountDB", {
				// DBNAME
				{"DBNAME", "OPCRUX_AD"},
				// ENCRYPTION
				{"ENCRYPTION", "sha512"}
			}},

			// [DBKeyDB]
			{"DBKeyDB", {
				// DBNAME
				{"DBNAME", "OPCRUX_KD"}
			}},

			// [ExtAccountService]
			{"ExtAccountService", {
				// EXTAUTH
				{"ExtAccountService", "FALSE"},
				// HOSTURL
				{"HOSTURL", "https://localhost:8443"},
				// APPKEY
				{"APPKEY", "THIS_IS_A_DUMMY_KEY"}
			}},

			{"API", {
				// Whether to use SSL TRUE or FALSE
				{"USESSL", "FALSE"}
			}}
		};}
	};
}

#endif