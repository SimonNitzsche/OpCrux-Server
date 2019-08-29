#ifndef __UTILS__LDFUTILS_HPP__
#define __UTILS__LDFUTILS_HPP__
#ifdef OPCRUX_PLATFORM_WIN32
	#define LDF_GET_VAL_FROM_COLLECTION(cVar, collection, keyStr, cDefault) {if (collection->find(keyStr) != collection->end()) {cVar = collection->at(keyStr);} else {cVar = cDefault;}}
#else
	#define LDF_GET_VAL_FROM_COLLECTION(cVar, collection, keyStr, cDefault) {if (collection->find(keyStr) != collection->end()) {cVar = collection->at(keyStr)[keyStr];} else {cVar = cDefault;}}
#endif

#include <string>
#include <memory>

#include "DataTypes/LDF.hpp"

class LDFUtils {
public:
	static LDFEntry MakeLDFEntryFromWStringData(std::wstring key, Enums::LDFType type, std::wstring val) {
		switch (type) {
		case Enums::LDFType::WSTRING: {
			return LDFEntry(key, val);
			break;
		}
		case Enums::LDFType::S32: {
			return LDFEntry(key, (std::int32_t)std::stoi(val));
			break;
		}
		case Enums::LDFType::FLOAT: {
			return LDFEntry(key, std::stof(val));
			break;
		}
		case Enums::LDFType::DOUBLE: {
			return LDFEntry(key, std::stod(val));
			break;
		}
		case Enums::LDFType::U32: {
			return LDFEntry(key, (std::uint32_t)std::stoi(val));
			break;
		}
		case Enums::LDFType::BOOLEAN: {
			return LDFEntry(key, val == L"1");
			break;
		}
		case Enums::LDFType::S64: {
			return LDFEntry(key, (std::int64_t)std::stoll(val));
			break;
		}
		case Enums::LDFType::LWOOBJID: {
			return LDFEntry(key, (DataTypes::LWOOBJID)std::stoll(val));
			break;
		}
		case Enums::LDFType::STRING: {
			std::wstring dummyW = val;
			std::string dummyS = std::string(dummyW.begin(), dummyW.end());
			return LDFEntry(key, dummyS);
			break;
		}
		default: {
			throw new std::runtime_error("Unable to recognize LDFType");
		}
		}
	}
	static LDFCollection ParseCollectionFromWString(std::wstring input) {
		LDFCollection output;
		std::vector<std::wstring> entries = StringUtils::splitWString(input,0x000a);
		for (int i = 0; i < entries.size(); ++i) {
			std::wstring entry = entries.at(i);
			std::vector<std::wstring> key_typeVal = StringUtils::splitWString(entry,0x003d);
			std::vector<std::wstring> type_val = StringUtils::splitWString(key_typeVal.at(1), 0x003a);

			Enums::LDFType type = (Enums::LDFType)std::stoi(type_val.at(0));

			LDFEntry e;

			
			if (type_val.size() == 1) type_val.push_back(L"");

			e = MakeLDFEntryFromWStringData(key_typeVal.at(0), type, type_val.at(1));
			
			output.insert({e.key, e });
		}
		return output;
	}
	static LDFEntry ReadEntryFromLUZWString(std::wstring key, std::wstring val) {
		std::vector<std::wstring> type_val = StringUtils::splitWString(val, 0x003a);
		Enums::LDFType type = (Enums::LDFType)std::stoi(type_val.at(0));
		if (type_val.size() == 1) type_val.push_back(L"");
		return MakeLDFEntryFromWStringData(key, type, type_val.at(1));
	}
};

#endif 