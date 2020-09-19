#include "FileTypes/LVLFile/LUScene.hpp"
#include "FileTypes/LUZFile/LUZone.hpp"

LUScene::LUScene(FileTypes::LUZ::LUZone * zone, std::string file) {
	Logger::log("WRLD", "Loading Scene: " + file);
	std::uint32_t size;
	filePtr = FileUtils::ReadFileCompletely(file, &size);
	char * data = reinterpret_cast<char*>(filePtr.get());
	std::uint32_t i = 0;
	while (i < size) {
		// Fix padding
		i += (i % 16);

		// Check magic
		char * magic = (data + i);
		if (std::string(magic, 4) != "CHNK") {
			// If we are at the start of the file, it's okay,
			// because it's the old non-chunked format.
			if (i == 0) {
				//std::uint32_t dOff = 0;
				//std::uint16_t lvlVersion = *reinterpret_cast<std::uint16_t*>(data);
				//std::uint32_t lvlVersion32 = static_cast<std::uint32_t>(lvlVersion);
				//dOff += 265;
				//for (int j = 0; j < 6; ++j) {
				//	std::uint32_t tmpCount = *reinterpret_cast<std::uint32_t*>(data + dOff);
				//	dOff += tmpCount + 4;
				//}
				//dOff += 4;

				////std::uint32_t tmpCount2 = *reinterpret_cast<std::uint32_t*>(data + dOff);
				////dOff += tmpCount2*3 + 4;

				//this->objectsChunk = LVLObjectsChunk(&lvlVersion32, data + dOff);


				throw std::runtime_error("TODO: Implement old level format!");

				//// Since it's not chunked no more chunks are read.
				return;
			}
			// Otherwise
			throw std::runtime_error("Level file is broken: Invalid chunk magic.");
		}

		// Read Header
		auto chunkHeader = LVLChunkHeader(data + i + 4);

		// Switch on chunk type so we know which one to add.
		// And obviously read it.
		switch (*chunkHeader.chunkType) {
		case ChunkType::INFO: {
			infoChunk = LVLInfoChunk(data + *(chunkHeader.chunkDataOffset));
			break;
		}
		case ChunkType::SKY: {
			break;
		}
		case ChunkType::OBJECTS: {
			objectsChunk = LVLObjectsChunk(infoChunk.version, data + *(chunkHeader.chunkDataOffset));
			break;
		}
		case ChunkType::ENV_CONFIG: {
			break;
		}
		default: {
			throw std::runtime_error("File corruption: Invalid chunk type.");
			break;
		}
		}

		// Seek index to next chunk.
		i += *chunkHeader.chunkTotalLength;
	}
}