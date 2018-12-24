#ifndef _LUZONE_HPP__
#define _LUZONE_HPP__

#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
#include "DataTypes/Position.hpp"
using namespace DataTypes;

struct ZoneString {
public:
	uint8_t * length = 0;;
	char * data = nullptr;
	uint8_t * Read(uint8_t * offset) {
		length = reinterpret_cast<uint8_t*>(offset);
		data = reinterpret_cast<char*>(offset + 1);
		return reinterpret_cast<uint8_t*>(data + *length);
	};
	std::string ToString() {
		return std::string(data, *length);
	};
};

struct ZoneWString {
public:
	uint8_t * length = 0;
	wchar_t * data = nullptr;
	uint8_t * Read(uint8_t * offset) {
		length = reinterpret_cast<uint8_t*>(offset);
		data = reinterpret_cast<wchar_t*>(offset + 1);
		return reinterpret_cast<uint8_t*>(data + *length);
	};
	std::wstring ToString() {
		return std::wstring(data, *length);
	};
};

struct SceneData {
	ZoneString fileName;
	uint32_t * sceneID;
	uint32_t * isAudioScene;
	ZoneString sceneName;
};

struct TerrainInfo {
	ZoneString fileName;
	ZoneString name;
	ZoneString description;
};

struct SceneTransitionPoint {
	uint64_t sceneID;
	Vector3 postition;
};

struct SceneTransition {
	ZoneString transitionName;
	std::vector<SceneTransitionPoint*> points;
};

class LUZone {
private:
	std::unique_ptr<unsigned char[]> filePtr;
	unsigned char* data;
private:
	uint32_t * version;
	uint32_t * unknown1;
	uint32_t * zoneID;
	Position * spawnPos;
	std::vector<SceneData> scenes;
	TerrainInfo terrainInfo;
	std::vector<SceneTransition> sceneTransitions;
public:
	LUZone(const std::string& filename);
	~LUZone();
private:
	void Read();
};

#endif // !_LUZONE_HPP__