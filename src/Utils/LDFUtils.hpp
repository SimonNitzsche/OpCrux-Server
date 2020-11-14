#ifndef __UTILS__LDFUTILS_HPP__
#define __UTILS__LDFUTILS_HPP__
#ifdef OPCRUX_PLATFORM_WIN32
	#define LDF_GET_VAL_FROM_COLLECTION(cVar, collection, keyStr, cDefault) {if (collection->find(keyStr) != collection->end()) {cVar = static_cast<decltype(cVar)>(collection->at(keyStr));} else {cVar = cDefault;}}
#else
	#define LDF_GET_VAL_FROM_COLLECTION(cVar, collection, keyStr, cDefault) {if (collection->find(keyStr) != collection->end()) {cVar = static_cast<decltype(cVar)>(collection->at(keyStr));} else {cVar = cDefault;}}
#endif

#include <string>
#include <memory>

#include "DataTypes/LDF.hpp"

class LDFUtils {
public:
	static inline LDFEntry MakeLDFEntryFromWStringData(std::u16string key, Enums::LDFType type, std::u16string val) {
		LDFEntry e = LDFEntry();
		switch (type) {
		case Enums::LDFType::WSTRING: {
			e = LDFEntry(key, val);
			break;
		}
		case Enums::LDFType::S32: {
			e = LDFEntry(key, (std::int32_t)std::stoi(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::FLOAT: {
			e = LDFEntry(key, std::stof(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::DOUBLE: {
			e = LDFEntry(key, std::stod(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::U32: {
			e = LDFEntry(key, (std::uint32_t)std::stoi(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::BOOLEAN: {
			e = LDFEntry(key, val == u"1");
			break;
		}
		case Enums::LDFType::S64: {
			e = LDFEntry(key, (std::int64_t)std::stoll(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::LWOOBJID: {
			e = LDFEntry(key, (DataTypes::LWOOBJID)std::stoll(StringUtils::to_string(val)));
			break;
		}
		case Enums::LDFType::STRING: {
			std::u16string dummyW = val;
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
	static LDFCollection ParseCollectionFromWString(std::u16string input) {
		LDFCollection output;
		std::vector<std::u16string> entries = StringUtils::splitWString(input,0x000a);
		for (int i = 0; i < entries.size(); ++i) {
			std::u16string entry = entries.at(i);
			std::vector<std::u16string> key_typeVal = StringUtils::splitWString(entry,0x003d);
			std::vector<std::u16string> type_val = StringUtils::splitWString(key_typeVal.at(1), 0x003a);

			Enums::LDFType type = (Enums::LDFType)std::stoi(StringUtils::to_string(type_val.at(0)));

			LDFEntry e;

			
			if (type_val.size() == 1) type_val.push_back(u"");

			std::u16string valFinal = type_val.at(1);
			// Fuse value splited by ":"
			for (int j = 2; j < type_val.size(); ++j) {
				valFinal += u":" + type_val.at(j);
			}

			e = MakeLDFEntryFromWStringData(key_typeVal.at(0), type, valFinal);
			
			output.insert({e.key, e });
		}
		return output;
	}
	static std::u16string PackCollectionToWString(LDFCollection& collection) {
		std::u16string output = u"";
		for (auto it = collection.begin(); it != collection.end(); ++it) {
			if (it != collection.begin()) {
				output += char16_t(0x000a); // \n
			}
			LDFEntry * entry = &it->second;
			output += entry->key;
			output += char16_t(0x003d); // =
			std::string typeStr = std::to_string(int(entry->type));
			output += std::u16string(typeStr.begin(), typeStr.end());
			output += char16_t(0x003a); // :
			output += entry->GetValueAsWString();
		}
		// Null terminator to make sure
		//output += char16_t(0x0000);
		return output;
	}
	static LDFCollection MakeCollectionFromLDFVector(std::vector<LDFEntry> & vec) {
		LDFCollection output;
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			output.insert({ it->key, *it });
		}
		return output;
	}
	static LDFEntry ReadEntryFromLUZWString(std::u16string key, std::u16string val) {
		std::vector<std::u16string> type_val = StringUtils::splitWString(val, 0x003a);
		bool typeDefined = val.find(0x003a) != std::u16string::npos;
		Enums::LDFType type = typeDefined ? (Enums::LDFType)std::stoi(StringUtils::to_string(type_val.at(0))) : Enums::LDFType::WSTRING;
		if (type_val.size() == 1) {
			if (type == Enums::LDFType::STRING || type == Enums::LDFType::WSTRING)
				type_val.push_back(u"");
			else
				type_val.push_back(u"-1");
		}

		std::u16string valFinal = type_val.at(1);
		if (typeDefined) {
			// Fuse value splited by ":"
			for (int j = 2; j < type_val.size(); ++j) {
				valFinal += u":" + type_val.at(j);
			}
		}

		return MakeLDFEntryFromWStringData(key, type, typeDefined ? valFinal : type_val.at(0));
	}
};

#endif 
