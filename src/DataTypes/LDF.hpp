#ifndef __DATATYPES__LDF_HPP__
#define __DATATYPES__LDF_HPP__

#include <string>
#include <map>

#include "Utils/StringUtils.hpp"

// Pre-Declare
class LDFEntry;

// Used to store a set of LDFEntries, so we can change it
// if needed without digging up delcarations in all classes.
typedef std::unordered_map<std::wstring*, LDFEntry> LDFCollection;

namespace Enums {
	enum LDFType : std::uint8_t {
		WSTRING,
		S32,
		UNKNOWN_2,
		FLOAT,
		DOUBLE,
		U32,
		UNKNOWN_6,
		BOOLEAN,
		S64,
		LWOOBJID,
		UNKNOWN_10,
		UNKWOWN_11,
		UNKNOWN_12,
		STRING,
		UNKNOWN_14,
	};
}

class LDFEntry {
public:
	std::wstring key;
	Enums::LDFType type;
private:
	char * data = nullptr;

public:
	const char * getPointer() { return data; }
	void WriteToBitstream(RakNet::BitStream * bs) {
		switch(type) {
			// 1 byte
		case Enums::LDFType::BOOLEAN: {
			bs->Write(data, 8);
			break;
		}
			// 4 byte
		case Enums::LDFType::S32:
		case Enums::LDFType::U32:
		case Enums::LDFType::FLOAT: {
			std::uint32_t buffer = *reinterpret_cast<std::uint32_t*>(data);
			bs->Write(buffer);
			break;
		}
			// 8 byte
		case Enums::LDFType::S64:
		case Enums::LDFType::LWOOBJID:
		case Enums::LDFType::DOUBLE: {
			std::uint64_t buffer = *reinterpret_cast<std::uint64_t*>(data);
			bs->Write(buffer);
			break;
		}
			// V byte
		case Enums::LDFType::STRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>(data);
			char * cp = data + 4;
			std::string buffer(cp, size);
			bs->Write(size);
			bs->Write(buffer);
			break;
		}
		case Enums::LDFType::WSTRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>(data);
			wchar_t * wcp = reinterpret_cast<wchar_t*>(data + 4);
			std::wstring buffer(wcp, size);
			bs->Write(size);
			StringUtils::writeWstringToBitStream(bs, buffer, buffer.size());
			break;
		}
		}
	}
public:
	LDFEntry(std::wstring inputKey, std::wstring input) {
		key = inputKey;
		type = Enums::LDFType::WSTRING;
		data = (char*)malloc(input.size()*2+4);
		std::uint32_t size = input.size();
		memcpy(data, &size, 4);
		memcpy(data+4, input.c_str(), input.size() * 2);
	}

	LDFEntry(std::wstring inputKey, std::int32_t input) {
		key = inputKey;	
		type = Enums::LDFType::S32;
		data = (char*)malloc(4);
		memcpy((char*)data, &input, 4);
	}

	LDFEntry(std::wstring inputKey, std::float_t input) {
		key = inputKey;	
		type = Enums::LDFType::FLOAT;
		data = (char*)malloc(4);
		memcpy((char*)data, &input, 4);
	}

	LDFEntry(std::wstring inputKey, std::double_t input) {
		key = inputKey;	
		type = Enums::LDFType::DOUBLE;
		data = (char*)malloc(8);
		memcpy((char*)data, &input, 8);
	}

	LDFEntry(std::wstring inputKey, std::uint32_t input) {
		key = inputKey;	
		type = Enums::LDFType::U32;
		data = (char*)malloc(4);
		memcpy((char*)data, &input, 4);
	}

	LDFEntry(std::wstring inputKey, bool input) {
		key = inputKey;	
		type = Enums::LDFType::BOOLEAN;
		data = (char*)malloc(1);
		std::uint8_t buffer = input;
		memcpy((char*)data, &buffer, 1);
	}

	LDFEntry(std::wstring inputKey, std::int64_t input) {
		key = inputKey;	
		type = Enums::LDFType::S64;
		data = (char*)malloc(8);
		memcpy((char*)data, &input, 8);
	}

	LDFEntry(std::wstring inputKey, DataTypes::LWOOBJID input) {
		key = inputKey;	
		type = Enums::LDFType::LWOOBJID;
		data = (char*)malloc(8);
		memcpy((char*)data, &input, 8);
	}

	LDFEntry(std::wstring inputKey, std::string input) {
		key = inputKey;	
		type = Enums::LDFType::STRING;
		data = (char*)malloc(input.size() + 4);
		std::uint32_t size = input.size();
		memcpy((char*)data, &size, 4);
		memcpy((char*)data + 4, (char*)input.c_str(), input.size());
	}

	void Delete() {
		if (data) {
			char * local = data;
			delete[] local;
			data = nullptr;
		}
	}
};
#endif