#include "NavMeshManager.hpp"
#include "Utils/Logger.hpp"
#include "Utils/FileUtils.hpp"
#include "DataTypes/Vector3.hpp"

bool NavMeshManager::Initialise() {
	m_navMeshOwner = std::make_unique<dtNavMesh>();
	m_navMesh = m_navMeshOwner.get();
	if (!m_navMesh) {
		Logger::log("WRLD", "Could not create Detour navmesh", LogType::ERR);
		return false;
	}

	m_navMeshQueryOwner = std::make_unique<dtNavMeshQuery>();
	m_navMeshQuery = m_navMeshQueryOwner.get();
	if (!m_navMeshQuery) {
		Logger::log("WRLD", "Could not create Detour navmesh query", LogType::ERR);
		return false;
	}
	return true;
}

bool NavMeshManager::Release() {
	m_navMesh = nullptr;
	m_navMeshQuery = nullptr;
	return true;
}

bool NavMeshManager::Open(std::string filepath) {
	auto file = FileUtils::ReadFileCompletely(filepath);
	auto file_ptr = file.get();

	Initialise();
	if (file_ptr == nullptr) return false;


	// Read header
	header = *reinterpret_cast<NavMeshSetHeader*>(file_ptr);
	file_ptr += sizeof(NavMeshSetHeader);

	m_navMesh->init(&header.params);

	for (int i = 0; i < header.numTiles; ++i) {
		NavMeshTileHeader tileHeader = *reinterpret_cast<NavMeshTileHeader*>(file_ptr);
		file_ptr += sizeof(NavMeshTileHeader);
		
		m_navMesh->addTile(file_ptr, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		file_ptr += sizeof(tileHeader.dataSize);
	}

	return false;
}

DataTypes::Vector3 NavMeshManager::findRandomPointAroundCircle(DataTypes::Vector3 start, std::float_t maxDistance) {
	dtQueryFilter m_filter;
	m_filter.setIncludeFlags(0xffff);
	m_filter.setExcludeFlags(0);

	m_navMeshQuery->init(m_navMesh, 2048);
	//dtStatus m_status = m_navMeshQuery->findNearestPoly()
	return DataTypes::Vector3();
}