#ifndef __FILETYPES__HKXFILE__CLASSES__HKROOTLEVELCONTAINER_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKROOTLEVELCONTAINER_HPP__
#include <memory>
#include <string>
#include <string_view>
#include <iostream>
#include "FileTypes/HKXFile/hkxFile.hpp"
#include "FileTypes/HKXFile/hkTypeMember.hpp"
class hkRootLevelContainer {
public:

	class NamedVariant {
	public:
		std::string_view m_name;
		std::string_view m_className;
		std::uint32_t m_variant;
	};

	std::list<class NamedVariant> m_namedVariants;

	void Read(HKX::HKXFile * hkfile, unsigned char* fd, std::uint32_t off) {
		unsigned char* doff = fd + off;
		std::uint32_t arrsize = *reinterpret_cast<std::uint32_t*>(doff + 4);
		
		std::uint32_t offData = hkfile->GetDataPointerTarget(off);
		doff = fd + offData;
		unsigned char* poffData = fd + offData;


		for (auto itType = hkfile->hkTypes.begin(); itType != hkfile->hkTypes.end(); ++itType) {
			if (itType->name != "hkRootLevelContainer") continue;


			std::string_view main_type = hkTypeMember::ToStringView(itType->members.at(0).tag[0]);
			std::string_view sub_type = hkTypeMember::ToStringView(itType->members.at(0).tag[1]);

			doff = fd + off + itType->members[0].offset;

			for (auto itType2 = hkfile->hkTypes.begin(); itType2 != hkfile->hkTypes.end(); ++itType2) {
				if (itType2->name != itType->members[0].structure) continue;

				for (std::uint32_t i = 0; i < arrsize; ++i) {
					unsigned char* paoff = doff + 8 + itType->object_size + i * itType2->object_size;

					NamedVariant eyNamedVariant{};
					auto memb_name = itType2->members.at(0);
					unsigned char* pmaoff = paoff + memb_name.offset;
					pmaoff = fd + hkfile->GetDataPointerTarget(pmaoff - fd);
					eyNamedVariant.m_name = std::string_view{ reinterpret_cast<char*>(pmaoff) };

					auto memb_className = itType2->members.at(1);
					pmaoff = paoff + memb_className.offset;
					pmaoff = fd + hkfile->GetDataPointerTarget(pmaoff - fd);
					eyNamedVariant.m_className = std::string_view{ reinterpret_cast<char*>(pmaoff) };
					
					auto memb_variant = itType2->members.at(2);
					pmaoff = paoff + memb_variant.offset;
					//pmaoff = fd + hkfile->GetDataPointerTarget(pmaoff - fd + 4);
					m_namedVariants.push_back(eyNamedVariant);
				}

				break;
			}

			break;
		}
	}
};

#endif