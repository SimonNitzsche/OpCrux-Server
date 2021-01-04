#ifndef	__FILETYPES__RAWFILE__RAWHEADER_HPP__
#define __FILETYPES__RAWFILE__RAWHEADER_HPP__

#include <memory>

namespace RAW {
	class RAWHeader {
	public:
#pragma pack(push, 0)
		std::uint8_t version;
		std::uint16_t padding;
		std::uint32_t chunkCount;
		std::uint32_t width;
		std::uint32_t height;
#pragma(pack, pop)
	
		void Read(std::uint8_t** fileDataPtr) {
			version = **fileDataPtr;
			++(*fileDataPtr);
			padding = *reinterpret_cast<std::uint16_t*>(*fileDataPtr);
			*fileDataPtr += 2;
			chunkCount = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			width = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			height = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
		}
	};
};
#endif