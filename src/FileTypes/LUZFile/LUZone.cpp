#include "LUZone.hpp"
#include "Utils/FileUtils.hpp"

LUZone::LUZone(const std::string& filename) {
	data = FileUtils::ReadFileCompletely(filename);
	Read();
}
LUZone::~LUZone() {
	delete data;
}

void LUZone::Read() {
	// Head
	version = reinterpret_cast<uint32_t*>(data + 0);
	if (*version >= 0x24UL) {
		unknown1 = reinterpret_cast<uint32_t*>(version + 1);
	}
	else { unknown1 = nullptr; }

	zoneID = reinterpret_cast<uint32_t*>((unknown1 ? unknown1 + 1 : version + 2));
	spawnPos = reinterpret_cast<Position*>(zoneID + 1);

	uint8_t * currentOffset;
	// Scenes
	{
		int count(
			((*version<0x25) ?
				*(reinterpret_cast<uint8_t*>(spawnPos + 1))
				: *(reinterpret_cast<uint32_t*>(spawnPos + 1))
				)
		);

		currentOffset = reinterpret_cast<uint8_t*>(spawnPos) + 28 + ((*version<0x25) ? 1 : 4);

		for (int i = 0; i < count; ++i) {
			SceneData sd;

			currentOffset = sd.fileName.Read(currentOffset);
			sd.sceneID = reinterpret_cast<uint32_t*>(currentOffset);
			sd.isAudioScene = reinterpret_cast<uint32_t*>(sd.sceneID + 1);
			currentOffset = sd.sceneName.Read(reinterpret_cast<uint8_t*>(sd.isAudioScene + 1)) + 3;

			scenes.push_back(sd);
		}
	}

	// Terrain
	++currentOffset =
		terrainInfo.description.Read(
			terrainInfo.name.Read(
				terrainInfo.fileName.Read(currentOffset)
			)
		);

	int c = 3;
}