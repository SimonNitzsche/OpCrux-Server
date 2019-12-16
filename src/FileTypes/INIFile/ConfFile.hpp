#ifndef __FILETYPES__CONFFILE_HPP__
#define __FILETYPES__CONFFILE_HPP__

#include <string>
#include <map>
#include "Utils/FileUtils.hpp"

#define ConfSection std::map<std::string, std::string>
#define ConfFileStruct std::map<std::string, ConfSection>

class ConfFile {
public:
	virtual std::string GetFilename() const { return ""; }
	virtual std::string GetComment() const { return ""; }
	virtual ConfFileStruct GetConfTemplate() const { return {}; }
	ConfFileStruct _data = {};
public:
	std::string GetFullFileName() {
		return "conf/" + GetFilename();
	}

	void Reload() {
		Load();
	}

	void Load() {
		_data = GetConfTemplate();
		if (!FileUtils::FileExists(GetFullFileName())) {
			Create();
		}
		Read();
		Save();
	}

	void Read() {
		std::string filecontent = FileUtils::ReadTextFile(GetFullFileName());
		std::list<std::string> lines = {};
		std::string to;
		std::stringstream ss(filecontent);
		while (std::getline(ss, to)) {
			lines.push_back(to);
		}


		std::string currentSection = "";

			for (auto it = lines.begin(); it != lines.end(); ++it) {
				std::string& line = *it;

				// Remove beginning whitespace
				int realBegin = 0;
				while (realBegin < line.size() && (line.at(realBegin) == ' ' || line.at(realBegin) == '\t' || line.at(realBegin) == '\0')) {
					++realBegin;
				}

				// Empty line, skip
				if (realBegin >= line.size()) continue;

				// Ignore comments
				if (line.at(realBegin) == ';') {
					continue;
				}

				// Check if section
				if (line.at(realBegin) == '[') {
					++realBegin;
					int sectionEnd = realBegin;
					while (sectionEnd < line.size() && line.at(sectionEnd) != ']') {
						++sectionEnd;
					}
					currentSection = line.substr(realBegin, sectionEnd - realBegin);
					continue;
				}

				// Get Key
				int keyEnd = realBegin;
				while (keyEnd < line.size() && line.at(keyEnd) != '=') {
					++keyEnd;
				}

				int keySize = keyEnd - realBegin;
				std::string& key = (keySize == 0) ? "" : line.substr(realBegin, keySize);

				++keyEnd;

				// Get Value
				int sweeper = keyEnd;
				int whitespaceCount = 0;
				while (sweeper< line.size()) {
					if (line.at(sweeper) == ' ' || line.at(sweeper) == '\t') {
						++whitespaceCount;
					}
					else {
						whitespaceCount = 0;
					}
					++sweeper;
				}

				int valSize = sweeper - keyEnd - whitespaceCount;
				std::string& val = (valSize == 0) ? "" : line.substr(keyEnd, valSize);

				auto itS = _data.find(currentSection);
				if (itS == _data.end()) {
					if (currentSection == "") continue;
					_data.insert({ currentSection, {} });
					itS = _data.find(currentSection);
				}

				auto itK = itS->second.find(key);
				if (itK == itS->second.end()) {
					if (key == "") continue;
					itS->second.insert({ key, val });
					itK = itS->second.find(key);
				}

				itK->second = val;
			}
	}

	void Create() {
		Save();
	}

	void Save() {

		std::string fileoutput = "";

		if (GetComment() != "") {
			fileoutput += GetComment() + "\n\n";
		}

		for (auto it = _data.begin(); it != _data.end(); ++it) {
			fileoutput += "["+it->first+"]\n";

			for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
				fileoutput += it2->first + "=" + it2->second + "\n";
			}

			fileoutput += "\n";
		}

		FileUtils::SaveTextFile(GetFullFileName(), fileoutput);
	}

	std::string GetStringVal(std::string section, std::string key) {
		auto itS = _data.find(section);
		auto itK = itS->second.find(key);
		return itK->second;
	}
};

#endif