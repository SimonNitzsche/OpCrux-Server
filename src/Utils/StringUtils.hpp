#ifndef _STRINGUTILS_HPP__
#define _STRINGUTILS_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include <RakNet/BitStream.h>

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

	inline std::wstring readWStringFromBitStream(RakNet::BitStream * bitstream, int len = 33) {
		std::unique_ptr<char[]> buff = std::make_unique<char[]>(len * 2);
		bitstream->Read(buff.get(), len * 2);

		int i;
		for (i = 0; i < len; ++i) {
			if (*reinterpret_cast<uint16_t*>(buff.get() + (i * 2)) == 0x0000) break;
		}

		std::wstring returnVal = std::wstring(reinterpret_cast<wchar_t*>(buff.get()), i);

		return returnVal;
	};
}

#endif // !_STRINGUTILS_HPP__
