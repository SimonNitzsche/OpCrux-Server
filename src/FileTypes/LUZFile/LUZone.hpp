#ifndef __FILETYPES__LUZFILE__LUZONE_HPP__
#define __FILETYPES__LUZFILE__LUZONE_HPP__

#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
#include "DataTypes/Position.hpp"
using namespace DataTypes;

namespace FileTypes::LUZone {

	/*
		Define the a string in a zone with easy reading from the file.
	*/
	struct ZoneString {
		public:
			uint8_t * length = 0;;
			char * data = nullptr;

			/*
				Reads the Zone String.
			*/
			uint8_t * Read(uint8_t * offset) {
				// Get the Length.
				length = reinterpret_cast<uint8_t*>(offset);

				// Set the data.
				data = reinterpret_cast<char*>(offset + 1);

				// Return length.
				return reinterpret_cast<uint8_t*>(data + *length);
			};

			/*
				Returns a std::string of the data.
			*/
			std::string ToString() {
				return std::string(data, *length);
			};
	};

	/*
		Defines a WString in a zone with easy reading from a file.
	*/
	struct ZoneWString {
		public:
			uint8_t * length = 0;
			wchar_t * data = nullptr;

			/*
				Reads the Zone WString.
			*/
			uint8_t * Read(uint8_t * offset) {
				// Get the Length.
				length = reinterpret_cast<uint8_t*>(offset);

				// Get the Data.
				data = reinterpret_cast<wchar_t*>(offset + 1);

				// Return the length.
				return reinterpret_cast<uint8_t*>(data + *length);
			};

			/*
				Returns a std::wstring of the data.
			*/
			std::wstring ToString() {
				return std::wstring(data, *length);
			};
	};

	/*
		Contains Scene Information.
	*/
	struct SceneData {
		ZoneString fileName;
		uint32_t * sceneID;
		uint32_t * isAudioScene;
		ZoneString sceneName;
	};

	/*
		Contains Terrain Info.
	*/
	struct TerrainInfo {
		ZoneString fileName;
		ZoneString name;
		ZoneString description;
	};

	/*
		Contains Scene Transition Point Info.
	*/
	struct SceneTransitionPoint {
		uint64_t sceneID;
		Vector3 postition;
	};

	/*
		Contains a Scene Transition Info.
	*/
	struct SceneTransition {
		ZoneString transitionName;
		std::vector<SceneTransitionPoint*> points;
	};

	/*
		The LEGO Universe Zone Class.
	*/
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
			// Constructs a new LUZone, with the path to the filename.
			LUZone(const std::string& filename);
			~LUZone();
		private:
			void Read();
	};
}

#endif // !__FILETYPES__LUZFILE__LUZONE_HPP__
