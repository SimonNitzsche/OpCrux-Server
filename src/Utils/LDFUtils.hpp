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
	static inline LDFEntry MakeLDFEntryFromWStringData(std::wstring key, Enums::LDFType type, std::wstring val) {
		LDFEntry e = LDFEntry();
		switch (type) {
		case Enums::LDFType::WSTRING: {
			e = LDFEntry(key, val);
			break;
		}
		case Enums::LDFType::S32: {
			e = LDFEntry(key, (std::int32_t)std::stoi(val));
			break;
		}
		case Enums::LDFType::FLOAT: {
			e = LDFEntry(key, std::stof(val));
			break;
		}
		case Enums::LDFType::DOUBLE: {
			e = LDFEntry(key, std::stod(val));
			break;
		}
		case Enums::LDFType::U32: {
			e = LDFEntry(key, (std::uint32_t)std::stoi(val));
			break;
		}
		case Enums::LDFType::BOOLEAN: {
			e = LDFEntry(key, val == L"1");
			break;
		}
		case Enums::LDFType::S64: {
			e = LDFEntry(key, (std::int64_t)std::stoll(val));
			break;
		}
		case Enums::LDFType::LWOOBJID: {
			e = LDFEntry(key, (DataTypes::LWOOBJID)std::stoll(val));
			break;
		}
		case Enums::LDFType::STRING: {
			std::wstring dummyW = val;
			std::string dummyS = std::string(dummyW.begin(), dummyW.end());
			e = LDFEntry(key, dummyS);
			break;
		}
		default: {
			throw new std::runtime_error("Unable to recognize LDFType");
		}
		}
		return e;
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

			std::wstring valFinal = type_val.at(1);
			// Fuse value splited by ":"
			for (int j = 2; j < type_val.size(); ++j) {
				valFinal += L":" + type_val.at(j);
			}

			e = MakeLDFEntryFromWStringData(key_typeVal.at(0), type, valFinal);
			
			output.insert({e.key, e });
		}
		return output;
	}
	static LDFEntry ReadEntryFromLUZWString(std::wstring key, std::wstring val) {
		std::vector<std::wstring> type_val = StringUtils::splitWString(val, 0x003a);
		bool typeDefined = val.find(0x003a) != std::wstring::npos;
		Enums::LDFType type = typeDefined ? (Enums::LDFType)std::stoi(type_val.at(0)) : Enums::LDFType::WSTRING;
		if (typeDefined && type_val.size() == 1) {
			if (type == Enums::LDFType::STRING || type == Enums::LDFType::WSTRING)
				type_val.push_back(L"");
			else
				type_val.push_back(L"-1");
		}

		std::wstring valFinal = type_val.at(1);
		if (typeDefined) {
			// Fuse value splited by ":"
			for (int j = 2; j < type_val.size(); ++j) {
				valFinal += L":" + type_val.at(j);
			}
		}

		return MakeLDFEntryFromWStringData(key, type, typeDefined ? valFinal : type_val.at(0));
	}
};

#endif 
