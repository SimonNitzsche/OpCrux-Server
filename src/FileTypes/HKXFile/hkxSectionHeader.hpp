#ifndef __FILETYPES__HKXFILE__HKXSECTIONHEADER_HPP__
#define __FILETYPES__HKXFILE__HKXSECTIONHEADER_HPP__
#include <memory>
#include <string>

namespace HKX {
	struct hkxSectionHeader {
		char m_sectionTag[19];
		std::uint8_t m_nullByte;
		std::uint32_t m_absoluteDataStart;
		std::uint32_t m_localFixupsOffset;
		std::uint32_t m_globalFixupsOffset;
		std::uint32_t m_virtualFixupsOffset;
		std::uint32_t m_exportsOffset;
		std::uint32_t m_importsOffset;
		std::uint32_t m_endOffset;

		hkxSectionHeader() {}


		/// Size in bytes of data part
		inline std::int32_t getDataSize() const {
			return m_localFixupsOffset;
		}
		/// Size in bytes of intra section pointer patches
		inline std::int32_t getLocalSize() const {
			return m_globalFixupsOffset - m_localFixupsOffset;
		}
		/// Size in bytes of inter section pointer patches
		inline std::int32_t getGlobalSize() const {
			return m_virtualFixupsOffset - m_globalFixupsOffset;
		}
		/// Size in bytes of finishing table.
		inline std::int32_t getFinishSize() const {
			return m_exportsOffset - m_virtualFixupsOffset;
		}
		/// Size in bytes of exports table.
		inline std::int32_t getExportsSize() const {
			return m_importsOffset - m_exportsOffset;
		}
		/// Size in bytes of imports table.
		inline std::int32_t getImportsSize() const {
			return m_endOffset - m_importsOffset;
		}
	};
}
#endif