#ifndef __FILETYPES__HKXFILE__HKXHEADER_HPP__
#define __FILETYPES__HKXFILE__HKXHEADER_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkxHeader {
		std::uint32_t m_magic[2];
		std::uint32_t m_userTag;
		std::uint32_t m_fileVersion;
		std::uint8_t m_layoutRules[4];
		std::uint32_t m_numSections;
		std::uint32_t m_contentsSectionIndex;
		std::uint32_t m_contentsSectionOffset;
		std::uint32_t m_contentsClassNameSectionIndex;
		std::uint32_t m_contentsClassNameSectionOffset;

		char m_contentsVersion[16];

		std::uint32_t m_flags;
		std::uint32_t m_pad;

		hkxHeader() {}
		
	};
}
#endif