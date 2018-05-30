#ifndef _STRINGUTILS_HPP__
#define _STRINGUTILS_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdlib.h>

namespace StringUtils {

	inline std::vector<std::string> splitString(const std::string &s, char delim) {
		std::stringstream ss;
		ss.str(s);
		std::string item;
		std::vector<std::string> elems;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	inline bool replace(std::string& str, const std::string& from, const std::string& to) {
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	};
}

#endif // !_STRINGUTILS_HPP__
