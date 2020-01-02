#include "FileTypes/HKXFile/hkxFile.hpp"

using namespace HKX;

void HKX::HKXFile::LoadStructure(std::uint32_t & currentOffset, std::string_view type_name) {
	std::uint32_t startOffset = currentOffset;
	bool found = false;

	for (auto it = hkTypes.begin(); it != hkTypes.end(); ++it) {
		if (it->name != type_name) continue;
		std::uint32_t arrayOffset = startOffset + it->object_size;
		found = true;

		if (it->parent != nullptr) LoadStructure(currentOffset, it->parent->name);

		for (int i = 0; i < it->members.size(); ++i) {
			std::string_view main_type = hkTypeMember::ToStringView(it->members.at(i).tag[0]);
			std::string_view sub_type = hkTypeMember::ToStringView(it->members.at(i).tag[1]);

			currentOffset = startOffset + it->members.at(i).offset;

			if (main_type == "enum") main_type = sub_type;

			if (main_type == "struct") {
				LoadStructure(currentOffset, it->members.at(i).structure);
			}
			else if (main_type == "pointer") {
				for (auto it2 = data_pointers.begin(); it2 != data_pointers.end(); ++it2) {
					if (currentOffset == it2->abs_address) {
						// Move pointer to target
						currentOffset = it2->target_address;
						break;
					}
				}
			}
			else if (main_type == "array" || main_type == "simple_array") {
				currentOffset += 4;
				std::uint32_t count = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				if (count == 0) {
					// Array is empty, skip
					continue;
				}

				for (auto it2 = data_pointers.begin(); it2 != data_pointers.end(); ++it2) {
					if (currentOffset - 8 == it2->abs_address) {
						// Move pointer
						currentOffset = it2->target_address;
						break;
					}
				}

				std::uint32_t targetOffset = currentOffset;
				std::uint32_t size = 1;

				for (auto it2 = hkTypes.begin(); it2 != hkTypes.end(); ++it2) {
					if (it2->name == it->members.at(i).structure) {
						size = it2->object_size;
						break;
					}
				}

				for (int j = 0; j < count; ++j) {
					if (sub_type == "struct") {
						currentOffset = targetOffset * size;
						LoadStructure(currentOffset, it->members.at(i).structure);
					}
					else {
						if (sub_type != "pointer") {
							LoadElement(currentOffset, sub_type);
						}
					}
				}
			}

		}
	}
}

void HKX::HKXFile::LoadElement(std::uint32_t & currentOffset, std::string_view main_type) {
	if (main_type == "int16" || main_type == "uint16" || main_type == "half") {
		std::uint16_t value = *reinterpret_cast<std::uint16_t*>(this->file.get() + currentOffset);
		currentOffset += 2;
	}
	else if (main_type == "int32" || main_type == "uint32" || main_type == "ulong") {
		std::uint32_t value = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
		currentOffset += 4;
	}
	else if (main_type == "real") {
		std::float_t value = *reinterpret_cast<std::float_t*>(this->file.get() + currentOffset);
		currentOffset += 4;
	}
	else if (main_type == "string_pointer" || main_type == "cstring") {
		unsigned char * value = this->file.get() + *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
		currentOffset += 4;
	}
	else if (main_type == "variant") {
		std::pair<std::uint32_t, std::uint32_t> value = { *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset),* reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset + 4) };
		currentOffset += 8;
	}
	else if (main_type == "int64" || main_type == "uint64") {
		std::uint64_t value = *reinterpret_cast<std::uint64_t*>(this->file.get() + currentOffset);
		currentOffset += 8;
	}

	// TODO: Implement others
}

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
		// __types__
		if (!strcmp(sectionHeader->m_sectionTag, "__types__")) {
			// Local fixups
			// for (int i = 0; currentOffset < sectionHeader->m_absoluteDataStart + sectionHeader->m_globalFixupsOffset; ++i) {
				// i think we don't need those
			// }

			// Global fixups
			for (int i = 0; currentOffset < sectionHeader->m_absoluteDataStart + sectionHeader->m_virtualFixupsOffset; ++i) {
				currentOffset = (sectionHeader->m_absoluteDataStart + sectionHeader->m_globalFixupsOffset + i * 12);
				std::uint32_t address = 0;
				std::uint32_t type = 0;
				std::uint32_t meta_address;

				address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				if (address == -1) break;
				type = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				meta_address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				
				hkLink link;
				link.address_1 = sectionHeader->m_absoluteDataStart + address;
				link.address_2 = sectionHeader->m_absoluteDataStart + meta_address;
				link.type = type;

				this->type_links.push_back(link);
			}

			// Virtual fixups
			for (int i = 0; currentOffset < sectionHeader->m_absoluteDataStart + sectionHeader->m_exportsOffset; ++i) {
				currentOffset = (sectionHeader->m_absoluteDataStart + sectionHeader->m_virtualFixupsOffset + i * 12);
				std::uint32_t address = 0;
				std::uint32_t name_address;
				std::string_view type_name;

				address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 8;

				if ((address == 0 && i != 0) || address == -1) break;

				name_address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				
				currentOffset = (class_name_global_address + name_address);
				unsigned char* name = this->file.get() + currentOffset;
				std::uint8_t cnamelen;
				for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
				type_name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
				currentOffset += cnamelen + 1;

				currentOffset = sectionHeader->m_absoluteDataStart + address;

				hkType type;
				type.reset();
				type.address = sectionHeader->m_absoluteDataStart + address;
				type.class_name = type_name;

				if ((type_name != "hkClass") && (type_name != "hkClassEnum")) continue;

				currentOffset += 4;

				for (auto it = type_links.begin(); it != type_links.end(); ++it) {
					if (it->address_1 == currentOffset) {
						type.parent_address = it->address_2;
						break;
					}
				}

				currentOffset += 4;
				type.object_size = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				type.num_implemented_interfaces = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 8;

				type.declared_enums = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 8;

				std::uint32_t membernum = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				if (type_name == "hkClass") {
					currentOffset += 12;
					type.described_version = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
					currentOffset += 4;
				}

				name = this->file.get() + currentOffset;
				for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
				type.name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
				currentOffset += cnamelen + 1;

				int pad = 16 - (currentOffset % 16);
				currentOffset += (pad==16) ? 0 : pad;

				if (type_name == "hkClass") {
					// Sub enums
					std::vector<std::uint32_t> subs_sizes;
					for (int j = 0; j < type.declared_enums; ++j) {
						currentOffset += 8;
						std::uint32_t ssize = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
						currentOffset += 4;
						currentOffset += 8;
						subs_sizes.push_back(ssize);
					}
					for (int j = 0; j < type.declared_enums; ++j) {
						name = this->file.get() + currentOffset;
						for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
						type.sub_enum_names.push_back(std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen));
						currentOffset += cnamelen + 1;

						pad = 16 - (currentOffset % 16);
						currentOffset += (pad == 16) ? 0 : pad;

						std::vector<hkEnum> subs;
						subs.clear();
						
						pad = 16 - (currentOffset % 16);
						currentOffset += (pad == 16) ? 0 : pad;

						hkEnum en;
						for (int k = 0; k < subs_sizes[j]; ++k) {
							en.id = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
							currentOffset += 8;

							subs.push_back(en);
						}

						pad = 16 - (currentOffset % 16);
						currentOffset += (pad == 16) ? 0 : pad;

						for (int k = 0; k < subs_sizes[j]; ++k) {
							name = this->file.get() + currentOffset;
							for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
							subs.at(k).name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
							currentOffset += cnamelen + 1;

							pad = 16 - (currentOffset % 16);
							currentOffset += (pad == 16) ? 0 : pad;
						}
						type.sub_enums.push_back(subs);
					}

					// Type members
					hkTypeMember typeMember;
					for (int j = 0; j < membernum; ++j) {
						currentOffset += 4;
						typeMember.structure_address = 0;

						for (auto it = type_links.begin(); it != type_links.end(); ++it) {
							if (it->address_1 == currentOffset) {
								typeMember.structure_address = it->address_2;
								break;
							}
						}

						currentOffset += 8;

						typeMember.tag[0] = *reinterpret_cast<std::uint8_t*>(this->file.get() + currentOffset++);
						typeMember.tag[1] = *reinterpret_cast<std::uint8_t*>(this->file.get() + currentOffset++);

						typeMember.array_size = *reinterpret_cast<std::uint16_t*>(this->file.get() + currentOffset);
						currentOffset += 2;

						typeMember.struct_type = *reinterpret_cast<std::uint16_t*>(this->file.get() + currentOffset);
						currentOffset += 2;

						typeMember.offset = *reinterpret_cast<std::uint16_t*>(this->file.get() + currentOffset);
						currentOffset += 2;

						currentOffset += 4;
						type.members.push_back(typeMember);
					}

					for (int j = 0; j < membernum; ++j) {
						name = this->file.get() + currentOffset;
						for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
						type.members.at(j).name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
						currentOffset += cnamelen + 1;

						pad = 16 - (currentOffset % 16);
						currentOffset += (pad == 16) ? 0 : pad;
					}
				}
				else {
					// Enums
					hkEnum en;

					for (int j = 0; j < type.object_size; ++j) {
						en.id = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
						currentOffset += 4;
						currentOffset += 4;
						type.enums.push_back(en);
					}

					int pad = 16 - (currentOffset % 16);
					currentOffset += (pad == 16) ? 0 : pad;

					for (int j = 0; j < type.object_size; ++j) {
						name = this->file.get() + currentOffset;
						for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
						type.enums.at(j).name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
						currentOffset += cnamelen + 1;

						int pad = 16 - (currentOffset % 16);
						currentOffset += (pad == 16) ? 0 : pad;
					}
				}

				hkTypes.push_back(type);
			}

			// Link types
			for (auto it = type_links.begin(); it != type_links.end(); ++it) {
				it->type_parent = nullptr;
				it->type_node = nullptr;
			}

			for (auto it = hkTypes.begin(); it != hkTypes.end(); ++it) {
				if (it->class_name != "hkClass") continue;

				if (it->parent_address != 0) {
					for (auto it2 = hkTypes.begin(); it2 != hkTypes.end(); ++it2) {
						if (it->parent_address == it2->address) {
							it->parent = &(*it2);
							break;
						}
					}
				}

				for (int j = 0; j < it->members.size(); ++j) {
					if (it->members.at(j).structure_address != 0) {
						for (auto it2 = hkTypes.begin(); it2 != hkTypes.end(); ++it2) {
							if (it2->address == it->members.at(j).structure_address) {
								it->members.at(j).structure = it2->name;
								break;
							}
						}
					}
				}
			}
		}
		else
		// __data__
		if (!strcmp(sectionHeader->m_sectionTag, "__data__")) {
			for (int i = 0; currentOffset < sectionHeader->m_absoluteDataStart + sectionHeader->m_globalFixupsOffset; ++i) {
				currentOffset = sectionHeader->m_absoluteDataStart + sectionHeader->m_localFixupsOffset + i * 8;
				std::uint32_t address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				if (address == -1) break;
				std::uint32_t address_2 = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				hkPointer pointer;
				pointer.abs_address = address + sectionHeader->m_absoluteDataStart;
				pointer.target_address = address_2 + sectionHeader->m_absoluteDataStart;

				data_pointers.push_back(pointer);
			}

			for (int i = 0; currentOffset < sectionHeader->m_absoluteDataStart + sectionHeader->m_virtualFixupsOffset; ++i) {
				currentOffset = sectionHeader->m_absoluteDataStart + sectionHeader->m_globalFixupsOffset + i * 12;

				std::uint32_t address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				if (address == -1) break;
				std::uint32_t type = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				std::uint32_t meta_address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				hkPointer pointer;
				pointer.abs_address = sectionHeader->m_absoluteDataStart + address;
				pointer.target_address = sectionHeader->m_absoluteDataStart + meta_address;

				data_global_pointers.push_back(pointer);
			}
		
			for (int i = 0; (sectionHeader->m_absoluteDataStart + sectionHeader->m_virtualFixupsOffset + i * 12) < sectionHeader->m_absoluteDataStart + sectionHeader->m_exportsOffset; ++i) {
				currentOffset = sectionHeader->m_absoluteDataStart + sectionHeader->m_virtualFixupsOffset + i * 12;

				std::uint32_t address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;
				currentOffset += 4;

				if (address == -1) break;

				std::uint32_t name_address = *reinterpret_cast<std::uint32_t*>(this->file.get() + currentOffset);
				currentOffset += 4;

				std::uint32_t currentOffsetReferencePoint = currentOffset;
				
				currentOffset = class_name_global_address + name_address;

				auto name = this->file.get() + currentOffset;
				int cnamelen;
				for (cnamelen = 0; cnamelen < 256 && name[cnamelen]; ++cnamelen);
				std::string_view type_name = std::string_view(const_cast<const char*>(reinterpret_cast<char*>(name)), cnamelen);
				currentOffset += cnamelen + 1;

				currentOffset = sectionHeader->m_absoluteDataStart + address;

				LoadStructure(currentOffset, type_name);

				currentOffset = currentOffsetReferencePoint;
			}
		}
	}
}