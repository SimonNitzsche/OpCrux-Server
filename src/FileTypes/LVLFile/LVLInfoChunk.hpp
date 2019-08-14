#ifndef __FILETYPES__LVLFILE__LVLInfoChunk_HPP__
#define __FILETYPES__LVLFILE__LVLInfoChunk_HPP__
#include "FileTypes/LVLFile/LVLChunkHeader.hpp"

#include <memory>

class LVLInfoChunk {
public:
	std::uint32_t * version;
	std::uint32_t * revision;
	std::uint32_t * skyAdress;
	std::uint32_t * objectsAdress;
	std::uint32_t * envAdress;
public:
	LVLInfoChunk() {}
	LVLInfoChunk(char * data) {
		version = reinterpret_cast<std::uint32_t*>(data + 0);
		revision = reinterpret_cast<std::uint32_t*>(data + 4);
		skyAdress = reinterpret_cast<std::uint32_t*>(data + 8);
		objectsAdress = reinterpret_cast<std::uint32_t*>(data + 12);
		envAdress = reinterpret_cast<std::uint32_t*>(data + 16);
	}
};

#endif