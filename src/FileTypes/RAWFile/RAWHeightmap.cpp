#include "RAWHeightmap.hpp"
#include "RAWChunk.hpp"
void RAW::RAWHeightmap::Read(RAW::RAWChunk* chunk, std::uint8_t** fileDataPtr) {
	unknownA = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
	*fileDataPtr += 4;
	unknownB = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
	*fileDataPtr += 4;
	unknownC = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
	*fileDataPtr += 4;
	unknownD = *reinterpret_cast<std::uint32_t*>(*fileDataPtr);
	*fileDataPtr += 4;


	scaleFactor = *reinterpret_cast<std::float_t*>(*fileDataPtr);
	*fileDataPtr += 4;

	size_t elements = chunk->width * chunk->height;
	for (size_t i = 0; i < elements; ++i) {
		map.emplace_back(*reinterpret_cast<std::float_t*>(*fileDataPtr));
		*fileDataPtr += 4;
	}
}
