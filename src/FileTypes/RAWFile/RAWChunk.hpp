#ifndef	__FILETYPES__RAWFILE__RAWCHUNK_HPP__
#define __FILETYPES__RAWFILE__RAWCHUNK_HPP__

#include <memory>
#include <cmath>
#include "RAWHeightmap.hpp"

namespace RAW {
	class RAWChunk {
	public:
		std::uint32_t chunkIndex;
		std::uint32_t width;
		std::uint32_t height;
		std::float_t positionX;
		std::float_t positionZ;
		RAWHeightmap heightmap;

		void Read(std::uint8_t** fileDataPtr) {
			chunkIndex = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			width = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			height = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			positionX = *reinterpret_cast<std::float_t*>(*fileDataPtr);
			*fileDataPtr += 4;
			positionZ = *reinterpret_cast<std::float_t*>(*fileDataPtr);
			*fileDataPtr += 4;

			heightmap.Read(this, fileDataPtr);

			// We don't need the rest and just increment our pointer based on the content

			// Color map
			auto colormapSize32 = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4 + (colormapSize32 * colormapSize32 * 4);

			// Light map
			auto lightmapSize32 = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4 + (lightmapSize32);

			// Color map 2
			auto colormap2Size32 = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4 + (colormap2Size32 * colormap2Size32 * 4);

			// weird bitflag
			auto weirdBitflag = *reinterpret_cast<std::uint8_t*>(*fileDataPtr);
			++(*fileDataPtr);

			// Blend map
			auto blendmapSize32 = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			*fileDataPtr += 4 + (blendmapSize32);

			// some points
			auto pointCount = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			(*fileDataPtr) += 4 + (pointCount * 9 * 4);

			// the color map stuff
			(*fileDataPtr) += colormapSize32 * colormapSize32;

			// the stuff at the end
			auto endCounter = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
			(*fileDataPtr) += 4 + (2 * endCounter);
			if (endCounter != 0) {
				(*fileDataPtr) += 16 * 2;
				for (int i = 0; i < 0x10; ++i) {
					auto thisIsTheFinalCountdown = *reinterpret_cast<std::uint16_t*>(*fileDataPtr);
					(*fileDataPtr) += thisIsTheFinalCountdown * 2;
				}
			}
		}
	};
};
#endif