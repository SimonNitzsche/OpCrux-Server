#ifndef __UTILS__LDFUTILS_HPP__
#define __UTILS__LDFUTILS_HPP__

#include <string>
#include <memory>
#include "DataTypes/LDF.hpp"

class LDFUtils {
	static LDFCollection ParseCollectionFromWString(std::wstring input) {
		LDFCollection output;
		std::vector<std::wstring> entries = StringUtils::splitWString(input,0x000a);
		for (int i = 0; i < entries.size(); ++i) {
			std::wstring entry = entries.at(i);
			std::vector<std::wstring> key_typeVal = StringUtils::splitWString(entry,0x003d);
			std::vector<std::wstring> type_val = StringUtils::splitWString(key_typeVal.at(1), 0x003a);

			LDFType type = (LDFType)std::stoi(type_val.at(0));

			LDFEntry * e;

			switch (type) {
			case LDFType::WSTRING: {
				e = &LDFEntry(key_typeVal.at(0), type_val.at(1));
				break;
			}
			case LDFType::S32: {
				e = &LDFEntry(key_typeVal.at(0), (std::int32_t)std::stoi(type_val.at(1)));
				break;
			}
			case LDFType::FLOAT: {
				e = &LDFEntry(key_typeVal.at(0), std::stof(type_val.at(1)));
				break;
			}
			case LDFType::DOUBLE: {
				e = &LDFEntry(key_typeVal.at(0), std::stod(type_val.at(1)));
				break;
			}
			case LDFType::U32: {
				e = &LDFEntry(key_typeVal.at(0), (std::uint32_t)std::stoi(type_val.at(1)));
				break;
			}
			case LDFType::BOOLEAN: {
				e = &LDFEntry(key_typeVal.at(0), type_val.at(1)==L"1");
				break;
			}
			case LDFType::S64: {
				e = &LDFEntry(key_typeVal.at(0), (std::int64_t)std::stoll(type_val.at(1)));
				break;
			}
			case LDFType::LWOOBJID: {
				e = &LDFEntry(key_typeVal.at(0), (DataTypes::LWOOBJID)std::stoll(type_val.at(1)));
				break;
			}
			case LDFType::STRING: {
				e = &LDFEntry(key_typeVal.at(0), std::string(type_val.at(1).begin(), type_val.at(1).end()));
				break;
			}
			default: {
				throw new std::runtime_error("Unable to recognize LDFType");
			}
			}
			std::wstring * k = &e->key;
			output.insert({ k, *e });
		}
		
	}
};

#endif 
