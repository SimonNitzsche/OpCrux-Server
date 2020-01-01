#include "FileTypes/HKXFile/hkxFile.hpp"

using namespace HKX;

void HKXFile::Load(std::string& file) {
	this->file = FileUtils::ReadFileCompletely(file);

	if (this->file == nullptr) return;

	// Header
	this->m_header = reinterpret_cast<hkxHeader*>(this->file.get());

	if (this->m_header->m_magic[0] != 0x57e0e057) return;
	if (this->m_header->m_magic[1] != 0x10c0c010) return;

	// Flags need to be set to 1 on load
	this->m_header->m_flags = 1;

	std::uint32_t currentOffset = sizeof(hkxHeader);

	// Section Headers
	for (int sectionIndex = 0; sectionIndex < this->m_header->m_numSections; ++sectionIndex) {
		this->m_sectionHeaders.push_back(reinterpret_cast<hkxSectionHeader*>(this->file.get() + currentOffset));
		currentOffset += sizeof(hkxSectionHeader);
	}

	// Sections
	for (int sectionIndex = 0; sectionIndex < this->m_sectionHeaders.size(); ++sectionIndex) {
		hkxSectionHeader* sectionHeader = *std::next(this->m_sectionHeaders.begin(), sectionIndex);
		currentOffset = sectionHeader->m_absoluteDataStart;

		// __classnames__
		if (!strcmp(sectionHeader->m_sectionTag, "__classnames__")) {
			hkClassName classname;
			class_name_global_address = sectionHeader->m_absoluteDataStart;

			for (int i = 0; classname.tag != 0xFFFFFFFF && *(this->file.get() + currentOffset) != sectionHeader->m_nullByte; ++i) {
				classname.name = "";
				classname.tag = reinterpret_cast<std::uint32_t>(this->file.get() + currentOffset);
				currentOffset += 5;

				if (classname.tag == 0xFFFFFFFF || classname.tag == 0x00000000) break;

				unsigned char* name = this->file.get() + currentOffset;
				std::uint8_t cnamelen;
				for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
				classname.name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
				currentOffset += cnamelen + 1;
				this->hkClassNames.push_back(classname);
			}

		}
		else
		if (!strcmp(sectionHeader->m_sectionTag, "__types__")) {

		}
		else
		if (!strcmp(sectionHeader->m_sectionTag, "__data__")) {

		}
	}
}