#ifndef __FILETYPES__LVLFILE__LVLChunkHeader_HPP__
#define __FILETYPES__LVLFILE__LVLChunkHeader_HPP__

#include <memory>

enum class ChunkType : std::uint32_t {
	INFO = 1000,
	SKY = 2000,
	OBJECTS = 2001,
	ENV_CONFIG = 2002
};

class LVLChunkHeader {
public:
	ChunkType * chunkType;
	std::uint32_t * unknown;
	std::uint32_t * chunkTotalLength;
	std::uint32_t * chunkDataOffset;

	LVLChunkHeader() {};
	LVLChunkHeader(char * data) {
		chunkType = reinterpret_cast<ChunkType*>(data);
		unknown = reinterpret_cast<std::uint32_t*>(data+4);
		chunkTotalLength = reinterpret_cast<std::uint32_t*>(data+8);
		chunkDataOffset = reinterpret_cast<std::uint32_t*>(data+12);
	}
};

/*
	Defines a WString in a zone with easy reading from a file.
*/
struct SceneWString {
public:
	uint32_t * length = 0;
	wchar_t * data = nullptr;

	/*
		Reads the Zone WString.
	*/
	char * Read(uint8_t * offset) {
		// Get the Length.
		length = reinterpret_cast<uint32_t*>(offset);

		// Get the Data.
		data = reinterpret_cast<wchar_t*>(offset + 4);

		// Return the length.
		return reinterpret_cast<char*>(data + *length *2);
	};

	/*
		Returns a std::wstring of the data.
	*/
	std::wstring ToString() {
		return std::wstring(data, *length);
	};
};

#endif