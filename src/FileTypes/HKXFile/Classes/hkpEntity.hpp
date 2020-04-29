#ifndef __FILETYPES__HKXFILE__CLASSES__HKPENTITY_HPP__
#define __FILETYPES__HKXFILE__CLASSES__HKPENTITY_HPP__

#include "hkpWorldObject.hpp"

class hkpEntity : public hkpWorldObject {
	HKX::HKXFile* hkfile; unsigned char* fd;
public:


	void Read(HKX::HKXFile* hkfile, unsigned char* fd, std::uint32_t off) {


		this->hkfile = hkfile; this->fd = fd;

		unsigned char* doff = fd + off;

		for (auto itType = hkfile->hkTypes.begin(); itType != hkfile->hkTypes.end(); ++itType) {
			if (itType->name != "hkpEntity") continue;

			hkpWorldObject::Read(hkfile, fd, off);

			// ...

			break;
		}
	}
};

#endif