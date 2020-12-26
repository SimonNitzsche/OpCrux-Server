#ifndef	__FILETYPES__RAWFILE__RAWCHUNK_HPP__
#define __FILETYPES__RAWFILE__RAWCHUNK_HPP__

#include <memory>
#include <cmath>
#include "RAWHeightmap.hpp"
#include "DataTypes/Vector3.hpp"
#include <sstream>
#include "Utils/FileUtils.hpp"

namespace RAW {
	class RAWChunk {
	public:
		class RAWChunkMesh {
		public:
			std::list<DataTypes::Vector3> verts;
			std::list<std::uint32_t> tris;
		};

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
					(*fileDataPtr) += 2 + thisIsTheFinalCountdown * 2;
				}
			}
		}

		RAWChunkMesh MakeUnoptimizedMesh() {
			RAWChunkMesh m;
			 
			for (int i = 0; i < width; ++i) {
				for (int j = 0; j < height; ++j) {
					std::float_t y = *std::next(heightmap.map.begin(), width * i + j);

					m.verts.push_back(DataTypes::Vector3(i, y, j));

					if (i == 0 || j == 0) continue;
					m.tris.push_back(width * i + j); //Top right
					m.tris.push_back(width * i + j - 1); //Bottom right
					m.tris.push_back(width * (i - 1) + j - 1); //Bottom left - First triangle

					m.tris.push_back(width * (i - 1) + j - 1); //Bottom left 
					m.tris.push_back(width * (i - 1) + j); //Top left
					m.tris.push_back(width * i + j); //Top right - Second triangle
					
				}
			}
			return m;
		}

		void SaveMesh(std::string& path) {
			auto m = MakeUnoptimizedMesh();

			std::stringstream ss;
			for (auto v : m.verts) {
				ss << "v " << v.x << " " << v.y << " " << v.z << "\n";
			}
			for (int i = 0; i < m.tris.size(); i += 3) {
				ss << "f " << *std::next(m.tris.begin(), i + 0) +1 << " " << *std::next(m.tris.begin(), i + 1) + 1 << " " << *std::next(m.tris.begin(), i + 2) + 1 << "\n";
			}

			FileUtils::SaveTextFile(path + ".obj", ss.str());
		}
	};
};
#endif