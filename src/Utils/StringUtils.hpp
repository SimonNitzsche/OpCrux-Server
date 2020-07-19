#ifndef __UTILS__STRINGUTILS_HPP__
#define __UTILS__STRINGUTILS_HPP__

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <memory>
#include <algorithm>
#include <cctype>
#include <locale>
#include <codecvt>
#include <list>

#include <RakNet/BitStream.h>
#include "Utils/RandomUtil.hpp"

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

	inline std::vector<std::u16string> splitWString(const std::u16string &s, char16_t delim) {
		std::basic_stringstream<char16_t> ss;
		ss.str(s);
		std::u16string item;
		std::vector<std::u16string> elems;
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

	template<class T>
	inline std::u16string readWStringFromBitStream(RakNet::BitStream * bitstream) {
		std::uint32_t size; bitstream->Read<T>(size);
		std::unique_ptr<char[]> buff = std::make_unique<char[]>(size * 2);
		bitstream->Read(buff.get(), size * 2);
		char16_t * data = reinterpret_cast<char16_t*>(buff.get());

		return std::u16string(data, size);
	}

	template<class T>
	inline std::string readStringFromBitStream(RakNet::BitStream * bitstream) {
		std::uint32_t size; bitstream->Read<T>(size);
		if (size == 0xcccccccc) throw std::runtime_error("Unable to read string!");
		std::unique_ptr<char[]> buff = std::make_unique<char[]>(size);
		bitstream->Read(buff.get(), size);
		char * data = static_cast<char*>(buff.get());

		return std::string(data, size);
	}

	inline std::u16string readBufferedWStringFromBitStream(RakNet::BitStream * bitstream, int len = 33) {
		std::unique_ptr<char[]> buff = std::make_unique<char[]>(len * 2);
		bitstream->Read(buff.get(), len * 2);

		char16_t * data = reinterpret_cast<char16_t*>(buff.get());

		int i;
		for (i = 0; i < len; ++i) {
			if (*(data + i) == 0x0000) break;
		}

		std::u16string returnVal = std::u16string(reinterpret_cast<char16_t*>(buff.get()), i);

		return returnVal;
	};

	inline std::string readBufferedStringFromBitStream(RakNet::BitStream * bitstream, int len = 33) {
		std::unique_ptr<char[]> buff = std::make_unique<char[]>(len);
		bitstream->Read(buff.get(), len);

		char * data = reinterpret_cast<char*>(buff.get());

		int i;
		for (i = 0; i < len; ++i) {
			if (*(data + i) == 0x00) break;
		}

		std::string returnVal = std::string(reinterpret_cast<char*>(buff.get()), i);

		return returnVal;
	};

	inline void FillZero(RakNet::BitStream * bitstream, int count) {
		bitstream->Write(std::string(count, 0x00).c_str(), count);
	}

	template<class T>
	inline void writeStringToBitStream(RakNet::BitStream * bitstream, std::string text) {
		std::uint32_t size = text.size();
		bitstream->Write<T>(size);

		bitstream->Write(text.c_str(), text.size());
	}

	template<class T>
	inline void writeWStringToBitStream(RakNet::BitStream * bitstream, std::u16string text, bool writeSizeOfBytes=false) {
		T size = text.size();

		if (writeSizeOfBytes) size *= 2;

		bitstream->Write<T>(size);

		bitstream->Write(reinterpret_cast<const char*>(text.c_str()), text.size() * 2);
	}

	inline void writeBufferedStringToBitStream(RakNet::BitStream * bitstream, std::string text, unsigned int len = 33) {
		if (text.length() > len) text = text.substr(0, len);
		bitstream->Write(text.c_str(), static_cast<const unsigned int>(text.length()));
		FillZero(bitstream, len - static_cast<unsigned int>(text.length()));
	}

	inline void writeBufferedWStringToBitStream(RakNet::BitStream * bitstream, std::u16string text, unsigned int len = 33) {
		if (text.length() > len) text = text.substr(0, len);
		bitstream->Write(reinterpret_cast<const char*>(text.c_str()), text.length() * 2);
		FillZero(bitstream, (len - text.length()) * 2);
	}

	inline void ToLowerSelf(std::string &data) {
		std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) -> unsigned char {	
			return std::tolower(c);
		});
	}

	inline std::string ToLower(std::string data) {
		std::string out(data);
		ToLowerSelf(out);
		return out;
	}

	inline char NibleToHex(char nible) {
		static const char* hexLookup = "0123456789ABCDEF";
		return hexLookup[nible&0xf];
	}

	inline std::string CharToHex(char c) {
		std::string o = "";
		o += NibleToHex(c >> 4);
		o += NibleToHex(c & 0x0F);
		return o;
	}

	inline std::string StringToHex(std::string input, char divider = '\0') {
		std::string out = "";
		for (int i = 0; i < input.size(); ++i) {
			if (i != 0) out += divider;
			out += CharToHex(input.at(i));
		}
	}

	inline std::string to_string(std::u16string input) {
		//std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
		//auto p = reinterpret_cast<const int16_t *>(input.data());
		//return convert.to_bytes(p, p + input.size());
		return std::string(input.begin(), input.end());
	}

	inline std::string IntListToString(std::list<std::int32_t> input, char fuser = ',') {
		std::string output = "";
		for (auto it = input.begin(); it != input.end(); ++it) {
			if (it != input.begin()) {
				output += fuser;
			}
			output += std::to_string(*it);
		}
		return output;
	}

	inline std::string StringVectorToString(std::vector<std::string> input, char fuser = ',') {
		std::string output = "";
		for (int i = 0; i < input.size(); ++i) {
			if (i != 0) {
				output += fuser;
			}
			output += input.at(i);
		}
		return output;
	}

	inline std::list<std::int32_t> StringVectorToIntList(std::vector<std::string> input){
		std::list<std::int32_t> output = {};
		for (int i = 0; i < input.size(); ++i) {
			output.push_back(std::stoi(input.at(i)));
		}
		return output;
	}

	inline std::u16string to_u16string(std::string input) {
		return std::u16string(input.begin(), input.end());
	}

	inline std::string generateRandomString(int max_length) {
		std::string possible_characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
		std::uniform_int_distribution<> dist(0, possible_characters.size() - 1);
		std::string ret = "";
		for (int i = 0; i < max_length; i++) {
			int random_index = dist(RandomUtil::GetEngine());
			ret += possible_characters[random_index];
		}
		return ret;
	}
	
	inline float StringToFloat(std::string arg) {
		return std::stof(arg);
	}

	inline int StringToInt(std::string arg) {
		return std::stoi(arg);
	}

	inline std::string FloatToString(float arg) {
		return std::to_string(arg);
	}

	inline std::string IntToString(int arg) {
		return std::to_string(arg);
	}
}

#endif // !__UTILS__STRINGUTILS_HPP__
