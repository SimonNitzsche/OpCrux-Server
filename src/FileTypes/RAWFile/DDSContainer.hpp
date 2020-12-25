#ifndef __FILETYPES__RAWFILE__DDSCONTAINER_HPP__
#define __FILETYPES__RAWFILE__DDSCONTAINER_HPP__
#include <memory>

namespace RAW {
	class DDSContainer {
		std::uint32_t size;
		std::uint8_t* data;

		void Read(std::uint8_t** fileDataPtr) {
			size = &static_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			data = *fileDataPtr;
			*fileDataPtr += size;
		}
	};
};
#endif