#ifndef __UTILS__STRINGUTILS_HPP__
#define __UTILS__STRINGUTILS_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <memory>

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

	inline void FillZero(RakNet::BitStream * bitstream, int count) {
		bitstream->Write(std::string(count, 0x00).c_str(), count);
	}

	inline void writeStringToBitStream(RakNet::BitStream * bitstream, std::string text, unsigned int len = 33) {
		if (text.length() > len) text = text.substr(0, len);
		bitstream->Write(text.c_str(), text.length());
		FillZero(bitstream, len - text.length());
	}

	inline void writeWstringToBitStream(RakNet::BitStream * bitstream, std::wstring text, unsigned int len = 33) {
		if (text.length() > len) text = text.substr(0, len);
		bitstream->Write(reinterpret_cast<const char*>(text.c_str()), text.length() * 2);
		FillZero(bitstream, (len - text.length()) * 2);
	}
}

#endif // !__UTILS__STRINGUTILS_HPP__
