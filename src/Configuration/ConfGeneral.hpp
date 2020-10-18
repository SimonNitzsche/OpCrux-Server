#ifndef __CONFIGURATION__CONFGENERAL_HPP__
#define __CONFIGURATION__CONFGENERAL_HPP__

#include "FileTypes/INIFile/ConfFile.hpp"

namespace Configuration {
	class ConfGeneral : public ConfFile {
	private:
		std::string GetFilename() const { return "General.ini"; }
		std::string GetComment() const {
			return \
				";==========================\n"\
				";General Configuration File\n"\
				";==========================\n";
		}
		ConfFileStruct GetConfTemplate() const {
			return {
				{"Master", {
					// In seconds
					{"TRANSFER_TIMEOUT", "30"},
					{"MASTERIP", "127.0.0.1"}
				}}
			};
		}
	};
}

#endif