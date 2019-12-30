#ifndef __CONFIGURATION__CONFPORTS_HPP__
#define __CONFIGURATION__CONFPORTS_HPP__

#include "FileTypes/INIFile/ConfFile.hpp"

namespace Configuration {
	class ConfPorts : public ConfFile {
	private:
		std::string GetFilename() const { return "NetworkPorts.ini"; }
		std::string GetComment() const {
			return \
				";===============================\n"\
				";Network Port Configuration File\n"\
				";===============================\n";
		}
		ConfFileStruct GetConfTemplate() const {
			return {
				// [Auth]
				{"Auth", {
					// PORT
					{"PORT", "1001"},
				}},

				// [World]
				{
					// PORTMIN
					{"PORTMIN", "2002"},
					// PORTMAX
					{"PORTMAX", "2199"}
				}

				
			};
		}
	};
}

#endif