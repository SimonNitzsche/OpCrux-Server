#ifndef __FILETYPES__HKXFILE__CLASSES__HKPPHYSICSDATA_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKPPHYSICSDATA_HPP__

#include "hkReferencedObject.hpp"
#include "hkpPhysicsSystem.hpp"

class hkpPhysicsData : public hkReferencedObject {
	HKX::HKXFile* hkfile; unsigned char* fd;
public:
	void Read(HKX::HKXFile* hkfile, unsigned char* fd, std::uint32_t off) {
		

		this->hkfile = hkfile; this->fd = fd;

		unsigned char* doff = fd + off;

		std::list<hkpPhysicsSystem> m_systems;

		for (auto itType = hkfile->hkTypes.begin(); itType != hkfile->hkTypes.end(); ++itType) {
			if (itType->name != "hkpPhysicsData") continue;

			hkReferencedObject::Read(hkfile, fd, off, itType->parent);
			
			auto memb_systems = itType->members.at(1);
			auto pmaoff = doff + memb_systems.offset;
			auto arrsize = *reinterpret_cast<std::uint32_t*>(pmaoff + 4);
			auto arrcontent = fd + hkfile->GetDataPointerTarget(pmaoff - fd);

			for (std::uint32_t i = 0; i < arrsize; ++i) {
				auto arrelementdata = hkfile->GetGlobalDataPointerTarget((arrcontent-fd) + i*8);

				hkpPhysicsSystem system;
				system.Read(hkfile, fd, arrelementdata);
				m_systems.push_back(system);

				int c = 3;
			}

			break;
		}
	}
};

#endif