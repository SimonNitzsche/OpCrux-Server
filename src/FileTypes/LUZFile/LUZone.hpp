#ifndef __FILETYPES__LUZFILE__LUZONE_HPP__
#define __FILETYPES__LUZFILE__LUZONE_HPP__

#include <string>
#include <vector>
#include <stdlib.h>
#include <memory>
#include "DataTypes/Position.hpp"
#include "DataTypes/LDF.hpp"

#include "FileTypes/LVLFile/LUScene.hpp"
using namespace DataTypes;

namespace FileTypes::LUZ {

	/*
		Define the a string in a zone with easy reading from the file.
	*/
	struct ZoneString {
		public:
			uint32_t length = 0;
			char * data = nullptr;

			/*
				Reads the Zone String.
			*/
			template<typename T = std::uint8_t>
			uint8_t * Read(uint8_t * offset) {
				// Get the Length.
				length = *reinterpret_cast<T*>(offset);

				// Set the data.
				data = reinterpret_cast<char*>(offset + sizeof(T));

				// Return length.
				return reinterpret_cast<uint8_t*>(data + length);
			};

			/*
				Returns a std::string of the data.
			*/
			std::string ToString() {
				return std::string(data, length);
			};
	};

	/*
		Defines a WString in a zone with easy reading from a file.
	*/
	struct ZoneWString {
		public:
			uint32_t length = 0;
			wchar_t * data = nullptr;

			/*
				Reads the Zone WString.
			*/
			template<typename T = std::uint8_t>
			uint8_t * Read(uint8_t * offset) {
				// Get the Length.
				length = *reinterpret_cast<uint8_t*>(offset);

				// Get the Data.
				data = reinterpret_cast<wchar_t*>(offset + sizeof(T));

				// Return the length.
				return reinterpret_cast<uint8_t*>(data + length);
			};

			/*
				Returns a std::wstring of the data.
			*/
			std::wstring ToString() {
				return std::wstring(data, length);
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
		LUScene scene = LUScene();
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
		The path type of a LUZ Path
	*/
	enum class LUZonePathType : std::uint32_t {
		Movement = 0,
		MovingPlatform = 1,
		Property = 2,
		Camera = 3,
		Spawner = 4,
		Showcase = 5,
		Race = 6,
		Rail = 7
	};

	/*
		The path behavior of a LUZ Path
	*/
	enum class LUZonePathBehaviour : std::uint32_t {
		Loop = 0,
		Bounce = 1,
		Once = 2
	};

	// Predeclare
	class LUZonePathWaypointBase;
	class LUZonePathBase;

	/*
		The base class every path's waypoint is deriving from
	*/
	class LUZonePathWaypointBase {
	public:
		DataTypes::Vector3 position;
	public:
		LDFCollection config{};
		LUZonePathWaypointBase() {}
	};

	/*
		The base class every path is deriving from
	*/
	class LUZonePathBase {
	public:
		std::uint32_t pathVersion;
		ZoneWString pathName;
		LUZonePathType pathType;
		std::uint32_t unknownBaseU32;
		LUZonePathBehaviour pathBehaviour;
	private:
		std::vector<LUZonePathWaypointBase*> waypoints {};
	public:
		LUZonePathBase() {}
	};

	/*
		The LUZ Path Waypoint for "Movement"
	*/
	class LUZonePathWaypointMovement : public LUZonePathWaypointBase {
	public:
		LUZonePathWaypointMovement() {}
	};

	/*
		The LUZ Path "Movement"
	*/
	class LUZonePathMovement : public LUZonePathBase {
	public:
		std::vector<LUZonePathWaypointMovement*> waypoints;
		LUZonePathMovement() {}
	};

	/*
		The LUZ Path Waypoint for "Moving Platform"
	*/
	class LUZonePathWaypointMovingPlatform : public LUZonePathWaypointBase {
	public:
		DataTypes::Quaternion rotation;
		bool lockPlayerUntilNextWaypoint;
		float speed;
		float wait;
		ZoneWString departSound;
		ZoneWString arriveSound;
		LUZonePathWaypointMovingPlatform() {}
	};

	/*
		The LUZ Path "Moving Platform"
	*/
	class LUZonePathMovingPlatform : public LUZonePathBase {
	public:
		std::uint8_t unknownByte;
		ZoneWString travelSound;
	public:
		std::vector<LUZonePathWaypointMovingPlatform*> waypoints;
		LUZonePathMovingPlatform() {}
	};

	/*
		The LUZ Path Waypoint for "Property"
	*/
	class LUZonePathWaypointProperty : public LUZonePathWaypointBase {
	public:
		LUZonePathWaypointProperty() {}
	};

	/*
		The LUZ Path "Property"
	*/
	class LUZonePathProperty : public LUZonePathBase {
	public:
		std::int32_t unknown1;
		std::int32_t price;
		std::int32_t rentalTime;
		std::uint64_t associatedZone;
		ZoneWString displayName;
		ZoneWString displayDescription;
		std::int32_t unknown2;
		std::int32_t cloneLimit;
		std::float_t reputationMultiplier;
		std::int32_t rentalTimeUnit;
		std::int32_t achievementRequired;
		DataTypes::Vector3 playerZonePosition;
		std::float_t maxBuildingHeight;
	public:
		std::vector<LUZonePathWaypointProperty*> waypoints;
		LUZonePathProperty() {}
	};

	/*
		The LUZ Path Waypoint for "Camera"
	*/
	class LUZonePathWaypointCamera : public LUZonePathWaypointBase {
	public:
		DataTypes::Quaternion rotation;
		float time=0;
		float unknown1=0;
		float tension=0;
		float continuity=0;
		float bias=0;
	public:
		LUZonePathWaypointCamera() {}
	};

	/*
		The LUZ Path "Camera"
	*/
	class LUZonePathCamera : public LUZonePathBase {
	public:
		ZoneWString nextPath;
		bool unknownBool;
	public:
		std::vector<LUZonePathWaypointCamera*> waypoints{};
		LUZonePathCamera() {}
	};

	/*
		The LUZ Path Waypoint for "Spawner"
	*/
	class LUZonePathWaypointSpawner : public LUZonePathWaypointBase {
	public:
		DataTypes::Quaternion rotation;
		LUZonePathWaypointSpawner() {}
	};

	/*
		The LUZ Path "Spawner"
	*/
	class LUZonePathSpawner : public LUZonePathBase {
	public:
		std::uint32_t spawnedLOT=0;
		std::uint32_t respawnTime=0;
		std::int32_t maxToSpawn=0;
		std::uint32_t numberToMaintain=0;
		std::uint64_t spawnerObjectID=0;
		bool activateSpawnerNetworkOnLoad=false;
	public:
		std::vector<LUZonePathWaypointSpawner*> waypoints;
		LUZonePathSpawner() {}
	};

	/*
		The LUZ Path Waypoint for "Showcase"
	*/
	class LUZonePathWaypointShowcase : public LUZonePathWaypointBase {
	public:
		LUZonePathWaypointShowcase() {}
	};

	/*
		The LUZ Path "Showcase"
	*/
	class LUZonePathShowcase : public LUZonePathBase {
	public:
		std::vector<LUZonePathWaypointShowcase*> waypoints;
		LUZonePathShowcase() {}
	};

	/*
		The LUZ Path Waypoint for "Race"
	*/
	class LUZonePathWaypointRace : public LUZonePathWaypointBase {
	public:
		DataTypes::Quaternion rotation;
		std::uint8_t unknown1;
		std::uint8_t unknown2;
		std::float_t unknown3;
		std::float_t unknown4;
		std::float_t unknown5;
	public:
		LUZonePathWaypointRace() {}
	};

	/*
		The LUZ Path "Race"
	*/
	class LUZonePathRace : public LUZonePathBase {
	public:
		std::vector<LUZonePathWaypointRace*> waypoints;
		LUZonePathRace() {}
	};

	/*
		The LUZ Path Waypoint for "Rail"
	*/
	class LUZonePathWaypointRail : public LUZonePathWaypointBase {
	public:
		DataTypes::Quaternion rotation;
		std::float_t speed;
	public:
		LUZonePathWaypointRail() {}
	};

	/*
		The LUZ Path "Rail"
	*/
	class LUZonePathRail : public LUZonePathBase {
	public:
		std::vector<LUZonePathWaypointRail*> waypoints;
		LUZonePathRail() {}
	};

	/*
		The LEGO Universe Zone Class.
	*/
	class LUZone {
		private:
			std::shared_ptr<unsigned char[]> filePtr;
			unsigned char* data;
			std::string strFile;
		public:
			uint32_t * version;
			uint32_t * unknown1;
			uint32_t * zoneID;
			Position * spawnPos;
			std::vector<SceneData> scenes;
			TerrainInfo terrainInfo;
			std::vector<SceneTransition> sceneTransitions;
			std::unordered_map<std::wstring, LUZonePathBase*> paths;
		public:
			// Constructs a new LUZone, with the path to the filename.
			LUZone(const std::string& filename);
			LUZonePathBase * AllocatePath(const LUZonePathType pathType);
			LUZonePathWaypointBase * AllocatePathWaypoint(const LUZonePathType pathType);
			LUZone();
			~LUZone();
		private:
			void Read();
	};
}

#endif // !__FILETYPES__LUZFILE__LUZONE_HPP__
