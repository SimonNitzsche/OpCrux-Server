#ifndef __FILETYPES__LVLFILE__LVLObjectsChunk_HPP__
#define __FILETYPES__LVLFILE__LVLObjectsChunk_HPP__
#include "FileTypes/LVLFile/LVLChunkHeader.hpp"
#include "DataTypes/Position.hpp"
#include <memory>
#include <list>

struct LVLObjectsChunkObject {
	std::uint64_t * objectID;
	std::int32_t * LOT;
	std::uint32_t * assetType;
	std::uint32_t * unknown;
	DataTypes::Position * spawnPos;
	std::float_t * scale;
	SceneWString config;
};

class LVLObjectsChunk {
public:
	std::uint32_t * objectCount;
	std::list<LVLObjectsChunkObject> objects;
public:
	LVLObjectsChunk() {}
	LVLObjectsChunk(std::uint32_t * version, char * data) {
		objectCount = reinterpret_cast<std::uint32_t*>(data + 0);
		std::uint32_t off = 4;
		for (std::uint32_t i = 0; i < *objectCount; ++i) {
			LVLObjectsChunkObject obj;
			obj.objectID = reinterpret_cast<std::uint64_t*>(data + off + 0);
			obj.LOT = reinterpret_cast<std::int32_t*>(data + off + 8);
			off += 12;
			if (*version >= 0x26) {
				obj.assetType = reinterpret_cast<std::uint32_t*>(data + off + 0);
				off += 4;
			}
			if (*version >= 0x20) {
				obj.unknown = reinterpret_cast<std::uint32_t*>(data + off + 0);
				off += 4;
			}
			obj.spawnPos = reinterpret_cast<DataTypes::Position*>(data + off + 0);
			obj.scale = reinterpret_cast<std::float_t*>(data + off + 28);
			obj.config.Read(reinterpret_cast<std::uint8_t*>(data + off + 32));
			off += 2 * (*obj.config.length) + 36;

			std::wstring test = obj.config.ToString();

			if (*version >= 7) {
				if (*reinterpret_cast<std::uint32_t*>(data + off) != 0x00000000) {
					throw new std::runtime_error("Invalid pointer position on lvl file.");
				}
				off += 4;
			}
			if (*obj.LOT != 0) {
				objects.push_back(obj);
			}
		}
	}
};

#endif