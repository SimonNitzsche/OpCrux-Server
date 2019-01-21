#include "LUZone.hpp"

#include "Utils/FileUtils.hpp"
using namespace FileTypes::LUZone;

LUZone::LUZone(const std::string& filename) {
	filePtr = FileUtils::ReadFileCompletely(filename);
	data = filePtr.get();
	Read();
}
LUZone::~LUZone() {}

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

		for (std::ptrdiff_t i = 0; i < count; ++i) {
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

	// Transisions
	if (*version >= 0x1f) {
		uint32_t * countOfScenes = reinterpret_cast<uint32_t*>(currentOffset);
		currentOffset = reinterpret_cast<uint8_t*>(countOfScenes+1);
		for (int transIndx = 0; transIndx < *countOfScenes; ++transIndx) {
			SceneTransition transitionFactory;
			// Transition Name
			if (*version < 0x25) {
				++currentOffset = transitionFactory.transitionName.Read(reinterpret_cast<uint8_t*>(currentOffset + 1));
			}
			
			// Transition Loops time
			uint8_t loopTimes = (*version <= 0x21 || *version >= 0x27) ? 2 : 5;

			// Transition Points
			for (int transPtIndx = 0; transPtIndx < loopTimes; ++transPtIndx) {
				SceneTransitionPoint * transPtFactory = reinterpret_cast<SceneTransitionPoint*>(currentOffset);
				transitionFactory.points.push_back(transPtFactory);
				currentOffset = reinterpret_cast<uint8_t*>(transPtFactory) + 20;
			}
			sceneTransitions.push_back(transitionFactory);
		}
	}
	
	// Paths
	if (*version >= 0x23) {
		uint32_t * lengthOfRestOfFile = reinterpret_cast<uint32_t*>(currentOffset);
		uint32_t * unknown1 = lengthOfRestOfFile + 1;
		uint32_t * countOfPaths = unknown1 + 1;
		currentOffset = reinterpret_cast<uint8_t*>(countOfPaths + 1);
		return;
		for (int pathIndx = 0; pathIndx < *countOfPaths; ++pathIndx) {
			uint32_t * pathVersion = reinterpret_cast<uint32_t*>(currentOffset);
			
			throw;
		}
	}
}
