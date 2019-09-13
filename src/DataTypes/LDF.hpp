#ifndef __DATATYPES__LDF_HPP__
#define __DATATYPES__LDF_HPP__

#include <string>
#include <unordered_map>

#include "DataTypes/LWOOBJID.hpp"

#include "Utils/StringUtils.hpp"

// Pre-Declare
class LDFEntry;

// Used to store a set of LDFEntries, so we can change it
// if needed without digging up delcarations in all classes.
typedef std::unordered_map<std::wstring, LDFEntry> LDFCollection;

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
	std::string data;

public:
	LDFEntry() {};
	const char * getPointer() { return data.c_str(); }
	void WriteToBitstream(RakNet::BitStream * bs) {
		switch(type) {
			// 1 byte
		case Enums::LDFType::BOOLEAN: {
			bs->Write(data.c_str(), 8);
			break;
		}
			// 4 byte
		case Enums::LDFType::S32:
		case Enums::LDFType::U32:
		case Enums::LDFType::FLOAT: {
			std::uint32_t buffer = *reinterpret_cast<std::uint32_t*>((char*)data.c_str());
			bs->Write(buffer);
			break;
		}
			// 8 byte
		case Enums::LDFType::S64:
		case Enums::LDFType::LWOOBJID:
		case Enums::LDFType::DOUBLE: {
			std::uint64_t buffer = *reinterpret_cast<std::uint64_t*>((char*)data.c_str());
			bs->Write(buffer);
			break;
		}
			// V byte
		case Enums::LDFType::STRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>((char*)data.c_str());
			char * cp = (char*)data.c_str() + 4;
			std::string buffer(cp, size);
			bs->Write(size);
			bs->Write(buffer);
			break;
		}
		case Enums::LDFType::WSTRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>((char*)data.c_str());
			wchar_t * wcp = reinterpret_cast<wchar_t*>((char*)data.c_str() + 4);
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
		data.resize(input.size() * 2 + 4);
		std::uint32_t size = input.size();
		memcpy((char*)data.c_str(), &size, 4);
		memcpy((char*)data.c_str()+4, input.c_str(), input.size() * 2);
	}

	LDFEntry(std::wstring inputKey, const wchar_t input[]) : LDFEntry(inputKey, std::wstring(input)) {}

	operator std::wstring() const {
		if (type != Enums::LDFType::WSTRING) throw new std::runtime_error("Invalid LDF type.");
		std::uint32_t * size = reinterpret_cast<std::uint32_t*>((char*)data.c_str());
		//return std::wstring(reinterpret_cast<wchar_t*>(data + 4), reinterpret_cast<wchar_t*>(data + 4 + (*size *2) -1));
		return std::wstring(reinterpret_cast<wchar_t*>((char*)data.c_str() + 4), *reinterpret_cast<std::uint32_t*>((char*)data.c_str()));
	}

	LDFEntry(std::wstring inputKey, std::int32_t input) {
		key = inputKey;	
		type = Enums::LDFType::S32;
		data.resize(4);
		memcpy((char*)data.c_str(), &input, 4);
	}

	operator std::int32_t() const {
		if (type != Enums::LDFType::S32) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::int32_t*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, std::float_t input) {
		key = inputKey;	
		type = Enums::LDFType::FLOAT; data.resize(4);
		memcpy((char*)data.c_str(), &input, 4);
	}

	operator std::float_t() const {
		if (type == Enums::LDFType::U32) return reinterpret_cast<std::uint32_t>(this);
		if (type != Enums::LDFType::FLOAT) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::float_t*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, std::double_t input) {
		key = inputKey;	
		type = Enums::LDFType::DOUBLE;
		data.resize(8);
		memcpy((char*)data.c_str(), &input, 8);
	}

	operator std::double_t() const {
		if (type != Enums::LDFType::DOUBLE) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::double_t*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, std::uint32_t input) {
		key = inputKey;	
		type = Enums::LDFType::U32;
		data.resize(4);
		memcpy((char*)data.c_str(), &input, 4);
	}

	operator std::uint32_t() const {
		if (type != Enums::LDFType::U32) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::uint32_t*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, bool input) {
		key = inputKey;	
		type = Enums::LDFType::BOOLEAN;
		data.resize(1);
		memcpy((char*)data.c_str(), &input, 1);
	}

	operator bool() const {
		if (type != Enums::LDFType::BOOLEAN) throw new std::runtime_error("Invalid LDF type.");
		return *data.c_str() != 0;
	}

	LDFEntry(std::wstring inputKey, std::int64_t input) {
		key = inputKey;	
		type = Enums::LDFType::S64;
		data.resize(8);
		memcpy((char*)data.c_str(), &input, 8);
	}

	operator std::int64_t() const {
		if (type != Enums::LDFType::S64) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::int64_t*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, DataTypes::LWOOBJID input) {
		key = inputKey;	
		type = Enums::LDFType::LWOOBJID;
		data.resize(8);
		memcpy((char*)data.c_str(), &input, 8);
	}

	operator DataTypes::LWOOBJID() const {
		if (type != Enums::LDFType::LWOOBJID) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<DataTypes::LWOOBJID*>((char*)data.c_str());
	}

	LDFEntry(std::wstring inputKey, std::string input) {
		key = inputKey;	
		type = Enums::LDFType::STRING;
		data = (char*)malloc(input.size() + 4);
		std::uint32_t size = input.size();
		data.resize(input.size() + 4);
		memcpy((char*)data.c_str(), &size, 4);
		memcpy((char*)data.c_str() + 4, (char*)input.c_str(), input.size());
	}

	LDFEntry(std::wstring inputKey, const char input[]) : LDFEntry(inputKey, std::string(input)) {}

	operator std::string() const {
		if (type != Enums::LDFType::STRING) throw new std::runtime_error("Invalid LDF type.");
		std::uint32_t * size = reinterpret_cast<std::uint32_t*>((char*)data.c_str());
		return std::string(reinterpret_cast<char*>((char*)data.c_str() + 4), reinterpret_cast<char*>((char*)data.c_str() + 4 + *size));
	}

	size_t getSize() const {
		switch (this->type) {
		case Enums::LDFType::BOOLEAN: {
			return 1;
		}
		case Enums::LDFType::U32:
		case Enums::LDFType::S32:
		case Enums::LDFType::FLOAT: {
			return 4;
		}
		case Enums::LDFType::S64:
		case Enums::LDFType::LWOOBJID:
		case Enums::LDFType::DOUBLE: {
			return 8;
		}
		case Enums::LDFType::STRING: {
			return 4 + *reinterpret_cast<std::uint32_t*>((char*)data.c_str());
		}
		case Enums::LDFType::WSTRING: {
			return 4 + 2**reinterpret_cast<std::uint32_t*>((char*)data.c_str());
		}
		}

		throw new std::runtime_error("Unknown LDF Type.");
		return 0;
	}

	~LDFEntry() {
	}
};
#endif