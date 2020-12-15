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
typedef std::unordered_map<std::u16string, LDFEntry> LDFCollection;

#define LDF_COLLECTION_INIT_ENTRY(name,var) {name, LDFEntry(name,var)}

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

		UNASSIGNED_LDFTYPE = 0xff
	};
}

class LDFEntry {
public:
	std::u16string key;
	Enums::LDFType type;
private:
	std::string data;

public:
	LDFEntry() :type(Enums::LDFType::UNASSIGNED_LDFTYPE) {};
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
			std::uint32_t buffer = *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
			bs->Write(buffer);
			break;
		}
			// 8 byte
		case Enums::LDFType::S64:
		case Enums::LDFType::LWOOBJID:
		case Enums::LDFType::DOUBLE: {
			std::uint64_t buffer = *reinterpret_cast<std::uint64_t*>(const_cast<char*>(data.c_str()));
			bs->Write(buffer);
			break;
		}
			// V byte
		case Enums::LDFType::STRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
			char * cp = const_cast<char*>(data.c_str()) + 4;
			std::string buffer(cp, size);
			StringUtils::writeStringToBitStream<std::uint32_t>(bs, buffer);
			break;
		}
		case Enums::LDFType::WSTRING: {
			std::uint32_t size = *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
			char16_t * wcp = reinterpret_cast<char16_t*>(const_cast<char*>(data.c_str()) + 4);
			std::u16string buffer(wcp, size);
			bs->Write(size);
			StringUtils::writeBufferedWStringToBitStream(bs, buffer, buffer.size());
			break;
		}
		}
	}
public:
	LDFEntry(std::u16string inputKey, std::u16string input) {
		key = inputKey;
		type = Enums::LDFType::WSTRING;
		data.resize(input.size() * 2 + 4);
		std::uint32_t size = input.size();
		memcpy(const_cast<char*>(data.c_str()), &size, 4);
		memcpy(const_cast<char*>(data.c_str())+4, input.c_str(), input.size() * 2);
	}

	LDFEntry(std::u16string inputKey, const char16_t input[]) : LDFEntry(inputKey, std::u16string(input)) {}

	explicit operator std::u16string() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return u"";
		if (type != Enums::LDFType::WSTRING) throw new std::runtime_error("Invalid LDF type.");
		std::uint32_t * size = reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
		//return std::u16string(reinterpret_cast<char16_t*>(data + 4), reinterpret_cast<char16_t*>(data + 4 + (*size *2) -1));
		return std::u16string(reinterpret_cast<char16_t*>(const_cast<char*>(data.c_str()) + 4), *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str())));
	}

	LDFEntry(std::u16string inputKey, std::int32_t input) {
		key = inputKey;	
		type = Enums::LDFType::S32;
		data.resize(4);
		memcpy(const_cast<char*>(data.c_str()), &input, 4);
	}

	explicit operator std::int32_t() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0;
		if (type != Enums::LDFType::S32) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::int32_t*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, std::float_t input) {
		key = inputKey;	
		type = Enums::LDFType::FLOAT; data.resize(4);
		memcpy(const_cast<char*>(data.c_str()), &input, 4);
	}

	explicit operator std::float_t() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0;
		if (type == Enums::LDFType::U32) return this->operator std::uint32_t();
		if (type != Enums::LDFType::FLOAT) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::float_t*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, std::double_t input) {
		key = inputKey;	
		type = Enums::LDFType::DOUBLE;
		data.resize(8);
		memcpy(const_cast<char*>(data.c_str()), &input, 8);
	}

	explicit operator std::double_t() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0;
		if (type != Enums::LDFType::DOUBLE) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::double_t*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, std::uint32_t input) {
		key = inputKey;	
		type = Enums::LDFType::U32;
		data.resize(4);
		memcpy(const_cast<char*>(data.c_str()), &input, 4);
	}

	explicit operator std::uint32_t() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0;
		if (type == Enums::LDFType::S32) return operator int32_t();
		if (type != Enums::LDFType::U32) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, bool input) {
		key = inputKey;	
		type = Enums::LDFType::BOOLEAN;
		data.resize(1);
		memcpy(const_cast<char*>(data.c_str()), &input, 1);
	}

	explicit operator bool() const {
		if (type != Enums::LDFType::BOOLEAN) return type != Enums::LDFType::UNASSIGNED_LDFTYPE;
		return *data.c_str() != 0;
	}

	LDFEntry(std::u16string inputKey, std::int64_t input) {
		key = inputKey;	
		type = Enums::LDFType::S64;
		data.resize(8);
		memcpy(const_cast<char*>(data.c_str()), &input, 8);
	}

	explicit operator std::int64_t() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0LL;
		if (type != Enums::LDFType::S64) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<std::int64_t*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, DataTypes::LWOOBJID input) {
		key = inputKey;	
		type = Enums::LDFType::LWOOBJID;
		data.resize(8);
		memcpy(const_cast<char*>(data.c_str()), &input, 8);
	}

	explicit operator DataTypes::LWOOBJID() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return 0ULL;
		if (type != Enums::LDFType::LWOOBJID) throw new std::runtime_error("Invalid LDF type.");
		return *reinterpret_cast<DataTypes::LWOOBJID*>(const_cast<char*>(data.c_str()));
	}

	LDFEntry(std::u16string inputKey, std::string input) {
		key = inputKey;	
		type = Enums::LDFType::STRING;
		std::uint32_t size = input.size();
		data.resize(input.size() + 4);
		memcpy(const_cast<char*>(data.c_str()), &size, 4);
		memcpy(const_cast<char*>(data.c_str()) + 4, const_cast<char*>(input.c_str()), input.size());
	}

	LDFEntry(std::u16string inputKey, const char input[]) : LDFEntry(inputKey, std::string(input)) {}

	explicit operator std::string() const {
		if (type == Enums::LDFType::UNASSIGNED_LDFTYPE) return "";
		if (type != Enums::LDFType::STRING) throw new std::runtime_error("Invalid LDF type.");
		std::uint32_t * size = reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
		return std::string(reinterpret_cast<char*>(const_cast<char*>(data.c_str()) + 4), reinterpret_cast<char*>(const_cast<char*>(data.c_str()) + 4 + *size));
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
			return 4 + *reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
		}
		case Enums::LDFType::WSTRING: {
			return 4 + 2**reinterpret_cast<std::uint32_t*>(const_cast<char*>(data.c_str()));
		}
		}

		throw new std::runtime_error("Unknown LDF Type.");
		return 0;
	}

	std::u16string GetValueAsWString() {
		switch (this->type) {
		case Enums::LDFType::BOOLEAN: {
			auto tmpVal = this->operator bool();
			std::string tmpValStr = std::to_string(int(tmpVal));
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::U32: {
			auto tmpVal = this->operator std::uint32_t();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::S32: {
			auto tmpVal = this->operator std::int32_t();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::FLOAT: {
			auto tmpVal = this->operator std::float_t();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::S64: {
			auto tmpVal = this->operator std::int64_t();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::LWOOBJID: {
			auto tmpVal = this->operator DataTypes::LWOOBJID();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::DOUBLE: {
			std::double_t tmpVal = this->operator double();
			std::string tmpValStr = std::to_string(tmpVal);
			return std::u16string(tmpValStr.begin(), tmpValStr.end());
		}
		case Enums::LDFType::STRING: {
			std::string tmpVal = this->operator std::string();
			return std::u16string(tmpVal.begin(), tmpVal.end());
		}
		case Enums::LDFType::WSTRING: {
			return this->operator std::u16string();
		}
		}

		throw new std::runtime_error("Unknown LDF Type.");
	}

	inline std::string GetValueAsString() {
		auto tmp = GetValueAsWString();
		return std::string(tmp.begin(), tmp.end());
	}

	~LDFEntry() {
	}
};
#endif