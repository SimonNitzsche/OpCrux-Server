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

			switch (type) {
			case Enums::LDFType::WSTRING: {
				e = LDFEntry(key_typeVal.at(0), type_val.at(1));
				break;
			}
			case Enums::LDFType::S32: {
				e = LDFEntry(key_typeVal.at(0), (std::int32_t)std::stoi(type_val.at(1)));
				break;
			}
			case Enums::LDFType::FLOAT: {
				e = LDFEntry(key_typeVal.at(0), std::stof(type_val.at(1)));
				break;
			}
			case Enums::LDFType::DOUBLE: {
				e = LDFEntry(key_typeVal.at(0), std::stod(type_val.at(1)));
				break;
			}
			case Enums::LDFType::U32: {
				e = LDFEntry(key_typeVal.at(0), (std::uint32_t)std::stoi(type_val.at(1)));
				break;
			}
			case Enums::LDFType::BOOLEAN: {
				e = LDFEntry(key_typeVal.at(0), type_val.at(1)==L"1");
				break;
			}
			case Enums::LDFType::S64: {
				e = LDFEntry(key_typeVal.at(0), (std::int64_t)std::stoll(type_val.at(1)));
				break;
			}
			case Enums::LDFType::LWOOBJID: {
				e = LDFEntry(key_typeVal.at(0), (DataTypes::LWOOBJID)std::stoll(type_val.at(1)));
				break;
			}
			case Enums::LDFType::STRING: {
				std::wstring dummyW = type_val.at(1);
				std::string dummyS = std::string(dummyW.begin(), dummyW.end());
				e = LDFEntry(key_typeVal.at(0), dummyS);
				break;
			}
			default: {
				throw new std::runtime_error("Unable to recognize LDFType");
			}
			}
			output.insert({e.key, e });
		}
		return output;
	}
};

#endif 
