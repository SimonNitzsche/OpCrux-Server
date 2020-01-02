#ifndef __FILETYPES__HKXFILE__HKTYPEMEMBER_HPP__
#define __FILETYPES__HKXFILE__HKTYPEMEMBER_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkTypeMember {
		std::uint8_t tag[2];
		std::uint16_t array_size;
		std::uint16_t struct_type;
		std::uint16_t offset;
		std::uint32_t structure_address;
		std::string_view name;
		std::string_view structure;

		hkTypeMember() {}

		static std::string_view ToStringView(std::uint8_t type) {
			switch (type) {
                case 0:
                    return "void";
                case 1:
                    return "bool";
                case 2:
                    return "char";
                case 3:
                    return "int8";
                case 4:
                    return "uint8";
                case 5:
                    return "int16";
                case 6:
                    return "uint16";
                case 7:
                    return "int32";
                case 8:
                    return "uint32";
                case 9:
                    return "int64";
                case 10:
                    return "uint64";
                case 11:
                    return "real";
                case 12:
                    return "vector4";
                case 13:
                    return "quaternion";
                case 14:
                    return "matrix3";
                case 15:
                    return "rotation";
                case 16:
                    return "qstransform";
                case 17:
                    return "matrix4";
                case 18:
                    return "transform";
                case 19:
                    return "zero";
                case 20:
                    return "pointer";
                case 21:
                    return "function_pointer";
                case 22:
                    return "array";
                case 23:
                    return "inplace_array";
                case 24:
                    return "enum";
                case 25:
                    return "struct";
                case 26:
                    return "simple_array";
                case 27:
                    return "homogeneous_array";
                case 28:
                    return "variant";
                case 29:
                    return "cstring";
                case 30:
                    return "ulong";
                case 31:
                    return "flags"; // hkFlags<ENUM,STORAGE> - 8,16,32 bits of named values.
                case 32:
                    return "half";
                case 33:
                    return "string_pointer";
                default:
                    return "UNIMPLEMENTED TYPE!!!";
			}
		}
	};
}
#endif