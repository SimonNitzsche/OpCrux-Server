#ifndef __FILETYPES__HKXFILE__CLASSES__HKPWORLDOBJECT_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKPWORLDOBJECT_HPP__

#include "hkReferencedObject.hpp"

class hkpWorldObject : public hkReferencedObject {
	HKX::HKXFile* hkfile; unsigned char* fd;
public:


	void Read(HKX::HKXFile* hkfile, unsigned char* fd, std::uint32_t off) {


		this->hkfile = hkfile; this->fd = fd;

		unsigned char* doff = fd + off;

		for (auto itType = hkfile->hkTypes.begin(); itType != hkfile->hkTypes.end(); ++itType) {
			if (itType->name != "hkpWorldObject") continue;

			hkReferencedObject::Read(hkfile, fd, off, itType->parent);

			// ...

			break;
		}
	}
};

#endif