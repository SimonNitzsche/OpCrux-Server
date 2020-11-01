#include "LUZone.hpp"

#include "Utils/FileUtils.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/LDFUtils.hpp"

using namespace FileTypes::LUZ;

LUZone::LUZone(const std::string& filename) {
	strFile = filename;
	filePtr = FileUtils::ReadFileCompletely(filename);
	data = filePtr.get();
	if (_isFileLoaded()) {
		Read();
	}
}

bool LUZone::_isFileLoaded() {
	return data != nullptr;
}

/*
	Calculate checksum
*/
std::uint32_t FileTypes::LUZ::LUZone::calculateRevisionChecksum() {
	Checksum cs;
	cs += 0xFFFFU;
	cs += 0;
	cs += revision;

	for (const auto& scene : scenes) {
		cs += *scene.sceneID;
		cs += *scene.layerID;
		cs += *scene.scene.infoChunk.revision;
	}

	return cs.result();
}

LUZonePathBase * FileTypes::LUZ::LUZone::AllocatePath(const LUZonePathType pathType) {
	switch (pathType) {
		case LUZonePathType::Movement:
			return new LUZonePathMovement();
		case LUZonePathType::MovingPlatform:
			return new LUZonePathMovingPlatform();
		case LUZonePathType::Property:
			return new LUZonePathProperty();
		case LUZonePathType::Camera:
			return new LUZonePathCamera();
		case LUZonePathType::Spawner:
			return new LUZonePathSpawner();
		case LUZonePathType::Showcase:
			return new LUZonePathShowcase();
		case LUZonePathType::Race:
			return new LUZonePathRace();
		case LUZonePathType::Rail:
			return new LUZonePathRail();			
	}
	throw new std::runtime_error("Invalid path type.");
}

LUZonePathWaypointBase * FileTypes::LUZ::LUZone::AllocatePathWaypoint(const LUZonePathType pathType) {
	switch (pathType) {
	case LUZonePathType::Movement:
		return new LUZonePathWaypointMovement();
	case LUZonePathType::MovingPlatform:
		return new LUZonePathWaypointMovingPlatform();
	case LUZonePathType::Property:
		return new LUZonePathWaypointProperty();
	case LUZonePathType::Camera:
		return new LUZonePathWaypointCamera();
	case LUZonePathType::Spawner:
		return new LUZonePathWaypointSpawner();
	case LUZonePathType::Showcase:
		return new LUZonePathWaypointShowcase();
	case LUZonePathType::Race:
		return new LUZonePathWaypointRace();
	case LUZonePathType::Rail:
		return new LUZonePathWaypointRail();
	}
	throw new std::runtime_error("Invalid path type.");
}

LUZone::LUZone() {}
LUZone::~LUZone() {
	for (auto pkp : paths) {

		switch (pkp.second->pathType) {
		case LUZonePathType::Movement:
			for (auto wp : reinterpret_cast<LUZonePathMovement*>(pkp.second)->waypoints) { delete wp; } break;
		case LUZonePathType::MovingPlatform:
			for(auto wp :reinterpret_cast<LUZonePathMovingPlatform*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Property:
			for(auto wp :reinterpret_cast<LUZonePathProperty*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Camera:
			for(auto wp :reinterpret_cast<LUZonePathCamera*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Spawner:
			for(auto wp :reinterpret_cast<LUZonePathSpawner*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Showcase:
			for(auto wp :reinterpret_cast<LUZonePathShowcase*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Race:
			for(auto wp :reinterpret_cast<LUZonePathRace*>(pkp.second)->waypoints) {delete wp;} break;
		case LUZonePathType::Rail:
			for(auto wp :reinterpret_cast<LUZonePathRail*>(pkp.second)->waypoints) {delete wp;} break;
		}

		delete pkp.second;
	}
}

void LUZone::Read() {

	uint8_t* currentOffset = data;
	
	// Head
	version = *reinterpret_cast<uint32_t*>(data + 0);
	currentOffset += 4;
	if (version >= 0x24UL) {
		revision = *reinterpret_cast<uint32_t*>(currentOffset);
		currentOffset += 4;
	}
	else { revision = 0; }

	zoneID = *reinterpret_cast<uint32_t*>((currentOffset));
	currentOffset += 4;
	if (version >= 0x26UL) {
		spawnPos = *reinterpret_cast<Position*>(currentOffset);

		float tmpX, tmpY, tmpZ, tmpW;
		tmpX = spawnPos.rot.w;
		tmpY = spawnPos.rot.x;
		tmpZ = spawnPos.rot.y;
		tmpW = spawnPos.rot.z;
		spawnPos.rot = DataTypes::Quaternion(tmpX, tmpY, tmpZ, tmpW);
		currentOffset += 28;
	}
	else {
		spawnPos = Position();
	}

	// Scenes
	{
		int count(
			((version<0x25) ?
				*(reinterpret_cast<uint8_t*>(currentOffset))
				: *(reinterpret_cast<uint32_t*>(currentOffset))
			)
		);

		currentOffset += ((version<0x25) ? 1 : 4);

		for (std::ptrdiff_t i = 0; i < count; ++i) {
			SceneData sd;

			currentOffset = sd.fileName.Read(currentOffset);
			sd.sceneID = reinterpret_cast<uint32_t*>(currentOffset);
			sd.layerID = reinterpret_cast<uint32_t*>(sd.sceneID + 1);
			currentOffset = sd.sceneName.Read(reinterpret_cast<uint8_t*>(sd.layerID + 1)) + 3;
			sd.scene = LUScene(this, StringUtils::ToLower(FileUtils::GetFileDir(this->strFile)+"/"+sd.fileName.ToString()));

			if (*sd.layerID == 0) {
				ZoneTriggerFile triggerFile{};
				if (triggerFile.Load(StringUtils::ToLower(FileUtils::GetFileDir(this->strFile) + "/" + sd.fileName.ToString().substr(0, sd.fileName.ToString().find_last_of(".")) + ".lutriggers").c_str())) {
					this->triggers.insert({ *sd.sceneID, triggerFile });
				}
			}

			scenes.push_back(sd);
		}
	}

	// Terrain
	++currentOffset;
	currentOffset = terrainInfo.fileName.Read(currentOffset);
	currentOffset = terrainInfo.name.Read(currentOffset);
	currentOffset = terrainInfo.description.Read(currentOffset);

	// Transisions
	if (version >= 0x1f) {
		uint32_t * countOfScenes = reinterpret_cast<uint32_t*>(currentOffset);
		currentOffset = reinterpret_cast<uint8_t*>(countOfScenes+1);
		for (int transIndx = 0; transIndx < *countOfScenes; ++transIndx) {
			SceneTransition transitionFactory;
			// Transition Name
			if (version < 0x25) {
				++currentOffset = transitionFactory.transitionName.Read(reinterpret_cast<uint8_t*>(currentOffset + 1));
			}
			
			// Transition Loops time
			uint8_t loopTimes = (version <= 0x21 || version >= 0x27) ? 2 : 5;

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
	if (version >= 0x23) {
		uint32_t * lengthOfRestOfFile = reinterpret_cast<uint32_t*>(currentOffset);
		uint32_t * unknown1 = lengthOfRestOfFile + 1;
		uint32_t * countOfPaths = unknown1 + 1;
		currentOffset = currentOffset + 12;

		for (int pathIndx = 0; pathIndx < *countOfPaths; ++pathIndx) {
			uint32_t * pathVersion = reinterpret_cast<uint32_t*>(currentOffset);
			ZoneWString pathName; currentOffset = pathName.Read(currentOffset + 4);
			LUZonePathType * pathType = reinterpret_cast<LUZonePathType*>(currentOffset);
			std::uint32_t * unknown2 = reinterpret_cast<std::uint32_t*>(currentOffset + 4);
			LUZonePathBehaviour * pathBehaviour = reinterpret_cast<LUZonePathBehaviour*>(currentOffset + 8);
			currentOffset += 12;

			LUZonePathBase * pathFactory = AllocatePath(*pathType);

			pathFactory->pathVersion = *pathVersion;
			pathFactory->pathName = pathName;
			pathFactory->pathType = *pathType;
			pathFactory->unknownBaseU32 = *unknown2;
			pathFactory->pathBehaviour = *pathBehaviour;

			// Note: we can't use switch here, as there are some if statements in the client
			// depending on version, that hasn't been added to the code below yet.

			if (*pathType == LUZonePathType::MovingPlatform) {
				LUZonePathMovingPlatform * pathMovingPlatform = reinterpret_cast<LUZonePathMovingPlatform*>(pathFactory);
				if (*pathVersion >= 18) {
					pathMovingPlatform->unknownByte = *reinterpret_cast<std::uint8_t*>(currentOffset++);
				}
				else if (*pathVersion >= 13) {
					currentOffset = pathMovingPlatform->travelSound.Read(reinterpret_cast<std::uint8_t*>(currentOffset++));
				}
			}
			else if (*pathType == LUZonePathType::Property) {
				LUZonePathProperty * pathProperty = reinterpret_cast<LUZonePathProperty*>(pathFactory);
				pathProperty->unknown1 = *reinterpret_cast<std::int32_t*>(currentOffset);
				pathProperty->price = *reinterpret_cast<std::int32_t*>(currentOffset + 4);
				pathProperty->rentalTime = *reinterpret_cast<std::int32_t*>(currentOffset + 8);
				pathProperty->associatedZone = *reinterpret_cast<std::int64_t*>(currentOffset + 12);
				currentOffset = pathProperty->displayName.Read(currentOffset + 20);
				currentOffset = pathProperty->displayDescription.Read<std::uint32_t>(currentOffset);
				pathProperty->unknown2 = *reinterpret_cast<std::int32_t*>(currentOffset);
				pathProperty->cloneLimit = *reinterpret_cast<std::int32_t*>(currentOffset + 4);
				pathProperty->reputationMultiplier = *reinterpret_cast<std::float_t*>(currentOffset + 8);
				pathProperty->rentalTimeUnit = *reinterpret_cast<std::int32_t*>(currentOffset + 12);
				pathProperty->achievementRequired = *reinterpret_cast<std::int32_t*>(currentOffset + 16);
				pathProperty->playerZonePosition = *reinterpret_cast<DataTypes::Vector3*>(currentOffset + 20);
				pathProperty->maxBuildingHeight = *reinterpret_cast<std::float_t*>(currentOffset + 32);
				currentOffset = currentOffset + 36;
			}
			else if (*pathType == LUZonePathType::Camera) {
				LUZonePathCamera * pathCamera = reinterpret_cast<LUZonePathCamera*>(pathFactory);
				currentOffset = pathCamera->nextPath.Read(reinterpret_cast<std::uint8_t*>(currentOffset));
				if (*pathVersion >= 14) {
					pathCamera->unknownBool = *reinterpret_cast<std::uint8_t*>(currentOffset++);
				}
			}
			else if (*pathType == LUZonePathType::Spawner) {
				LUZonePathSpawner * pathSpawner = reinterpret_cast<LUZonePathSpawner*>(pathFactory);
				pathSpawner->spawnedLOT = *reinterpret_cast<std::uint32_t*>(currentOffset);
				pathSpawner->respawnTime = *reinterpret_cast<std::uint32_t*>(currentOffset + 4);
				pathSpawner->maxToSpawn = *reinterpret_cast<std::int32_t*>(currentOffset + 8);
				pathSpawner->numberToMaintain = *reinterpret_cast<std::uint32_t*>(currentOffset + 12);
				pathSpawner->spawnerObjectID = *reinterpret_cast<std::int64_t*>(currentOffset + 16);
				currentOffset = currentOffset + 24;
				if (*pathVersion > 8) {
					pathSpawner->activateSpawnerNetworkOnLoad = *reinterpret_cast<std::uint8_t*>(currentOffset++);
				}
			}

			// Waypoints
			std::uint32_t waypointCount = *reinterpret_cast<std::uint32_t*>(currentOffset);
			currentOffset += 4;
			if (waypointCount > 10000)
				throw new std::runtime_error("File Corruption: Maximum amount of path waypoints has been exeeded.");
			for (int i = 0; i < waypointCount; ++i) {
				LUZonePathWaypointBase * waypointFactory = AllocatePathWaypoint(*pathType);
				waypointFactory->position = *reinterpret_cast<DataTypes::Vector3*>(currentOffset);
				currentOffset = currentOffset + 12;
				if (*pathType == LUZonePathType::MovingPlatform) {
					LUZonePathWaypointMovingPlatform * waypointMovingPlatform = reinterpret_cast<LUZonePathWaypointMovingPlatform*>(waypointFactory);
					waypointMovingPlatform->rotation = DataTypes::Quaternion (
						*reinterpret_cast<std::float_t*>(currentOffset + 4),
						*reinterpret_cast<std::float_t*>(currentOffset + 8),
						*reinterpret_cast<std::float_t*>(currentOffset + 12),
						*reinterpret_cast<std::float_t*>(currentOffset + 0)
					);
					waypointMovingPlatform->lockPlayerUntilNextWaypoint = *reinterpret_cast<std::uint8_t*>(currentOffset + 16);
					waypointMovingPlatform->speed = *reinterpret_cast<std::float_t*>(currentOffset + 17);
					waypointMovingPlatform->wait = *reinterpret_cast<std::float_t*>(currentOffset + 21);
					currentOffset = currentOffset + 25;
					if (*pathVersion >= 13) {
						currentOffset = waypointMovingPlatform->departSound.Read(reinterpret_cast<std::uint8_t*>(currentOffset));
						currentOffset = waypointMovingPlatform->arriveSound.Read(reinterpret_cast<std::uint8_t*>(currentOffset));
					}
				}
				else if (*pathType == LUZonePathType::Camera) {
					LUZonePathWaypointCamera * waypointCamera = reinterpret_cast<LUZonePathWaypointCamera*>(waypointFactory);
					waypointCamera->rotation = DataTypes::Quaternion(
						*reinterpret_cast<std::float_t*>(currentOffset + 4),
						*reinterpret_cast<std::float_t*>(currentOffset + 8),
						*reinterpret_cast<std::float_t*>(currentOffset + 12),
						*reinterpret_cast<std::float_t*>(currentOffset + 0)
					);
					currentOffset = currentOffset + 16;

					waypointCamera->time = *reinterpret_cast<std::float_t*>(currentOffset + 0);
					waypointCamera->unknown1 = *reinterpret_cast<std::float_t*>(currentOffset + 4);
					waypointCamera->tension = *reinterpret_cast<std::float_t*>(currentOffset + 8);
					waypointCamera->continuity = *reinterpret_cast<std::float_t*>(currentOffset + 12);
					waypointCamera->bias = *reinterpret_cast<std::float_t*>(currentOffset + 16);
					currentOffset = currentOffset + 20;
				}
				else if (*pathType == LUZonePathType::Spawner) {
					LUZonePathWaypointSpawner * waypointSpawner = reinterpret_cast<LUZonePathWaypointSpawner*>(waypointFactory);
					waypointSpawner->rotation = DataTypes::Quaternion(
						*reinterpret_cast<std::float_t*>(currentOffset + 4),
						*reinterpret_cast<std::float_t*>(currentOffset + 8),
						*reinterpret_cast<std::float_t*>(currentOffset + 12),
						*reinterpret_cast<std::float_t*>(currentOffset + 0)
					);
					currentOffset = currentOffset + 16;
				}
				else if (*pathType == LUZonePathType::Race) {
					LUZonePathWaypointRace * waypointRace = reinterpret_cast<LUZonePathWaypointRace*>(waypointFactory);
					waypointRace->rotation = DataTypes::Quaternion(
						*reinterpret_cast<std::float_t*>(currentOffset + 4),
						*reinterpret_cast<std::float_t*>(currentOffset + 8),
						*reinterpret_cast<std::float_t*>(currentOffset + 12),
						*reinterpret_cast<std::float_t*>(currentOffset + 0)
					);
					waypointRace->unknown1 = *reinterpret_cast<std::uint8_t*>(currentOffset + 16);
					waypointRace->unknown2 = *reinterpret_cast<std::uint8_t*>(currentOffset + 17);
					waypointRace->unknown3 = *reinterpret_cast<std::float_t*>(currentOffset + 18);
					waypointRace->unknown4 = *reinterpret_cast<std::float_t*>(currentOffset + 22);
					waypointRace->unknown5 = *reinterpret_cast<std::float_t*>(currentOffset + 26);
					currentOffset = currentOffset + 30;
				}
				else if (*pathType == LUZonePathType::Rail) {
					LUZonePathWaypointRail * waypointRail = reinterpret_cast<LUZonePathWaypointRail*>(waypointFactory);
					waypointRail->rotation = DataTypes::Quaternion(
						*reinterpret_cast<std::float_t*>(currentOffset + 4),
						*reinterpret_cast<std::float_t*>(currentOffset + 8),
						*reinterpret_cast<std::float_t*>(currentOffset + 12),
						*reinterpret_cast<std::float_t*>(currentOffset + 0)
					);
					currentOffset = currentOffset + 16;
					if (*pathVersion >= 17) {
						waypointRail->speed = *reinterpret_cast<std::float_t*>(currentOffset);
						currentOffset = currentOffset + 4;
					}
				}

				// config
				if (*pathType == LUZonePathType::Movement || *pathType == LUZonePathType::Spawner || *pathType == LUZonePathType::Rail) {
					std::uint32_t configCount = *reinterpret_cast<std::uint32_t*>(currentOffset);
					currentOffset = currentOffset + 4;
					for (int j = 0; j < configCount; ++j) {
						ZoneWString key, val;
						currentOffset = key.Read(reinterpret_cast<std::uint8_t*>(currentOffset));
						currentOffset = val.Read(reinterpret_cast<std::uint8_t*>(currentOffset));

						LDFEntry entry = LDFUtils::ReadEntryFromLUZWString(key.ToString(), val.ToString());
						waypointFactory->config.insert({ entry.key, entry });
					}
				}

				// Add Waypoint
				switch (*pathType) {
				case LUZonePathType::Movement: {
					reinterpret_cast<LUZonePathMovement*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointMovement*>(waypointFactory));
					break;
				}
				case LUZonePathType::MovingPlatform: {
					reinterpret_cast<LUZonePathMovingPlatform*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointMovingPlatform*>(waypointFactory));
					break;
				}
				case LUZonePathType::Property: {
					reinterpret_cast<LUZonePathProperty*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointProperty*>(waypointFactory));
					break;
				}
				case LUZonePathType::Camera: {
					reinterpret_cast<LUZonePathCamera*>(pathFactory)->waypoints.push_back(reinterpret_cast<LUZonePathWaypointCamera*>(waypointFactory));
					break;
				}
				case LUZonePathType::Spawner: {
					reinterpret_cast<LUZonePathSpawner*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointSpawner*>(waypointFactory));
					break;
				}
				case LUZonePathType::Showcase: {
					reinterpret_cast<LUZonePathShowcase*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointShowcase*>(waypointFactory));
					break;
				}
				case LUZonePathType::Race: {
					reinterpret_cast<LUZonePathRace*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointRace*>(waypointFactory));
					break;
				}
				case LUZonePathType::Rail: {
					reinterpret_cast<LUZonePathRail*>(pathFactory)->waypoints.push_back(static_cast<LUZonePathWaypointRail*>(waypointFactory));
					break;
				}
				}
			}

			// Add path to list
			
			switch (*pathType) {
			case LUZonePathType::Movement: {
				auto myPathInsert = reinterpret_cast<LUZonePathMovement*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::MovingPlatform: {
				auto myPathInsert = reinterpret_cast<LUZonePathMovingPlatform*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Property: {
				auto myPathInsert = reinterpret_cast<LUZonePathProperty*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Camera: {
				auto myPathInsert = reinterpret_cast<LUZonePathCamera*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Spawner: {
				auto myPathInsert = reinterpret_cast<LUZonePathSpawner*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Showcase: {
				auto myPathInsert = reinterpret_cast<LUZonePathShowcase*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Race: {
				auto myPathInsert = reinterpret_cast<LUZonePathRace*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			case LUZonePathType::Rail: {
				auto myPathInsert = reinterpret_cast<LUZonePathRail*>(pathFactory);
				paths.insert({myPathInsert->pathName.ToString(), myPathInsert});
				break;
			}
			}
		}
	}
	
	this->revisionChecksum = calculateRevisionChecksum();
}
