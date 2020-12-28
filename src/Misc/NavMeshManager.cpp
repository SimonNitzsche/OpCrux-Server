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
	if (file_ptr == nullptr) {
		Release();
		return false;
	}


	// Read header
	header = *reinterpret_cast<NavMeshSetHeader*>(file_ptr);
	file_ptr += sizeof(NavMeshSetHeader);

	m_navMesh->init(&header.params);

	for (int i = 0; i < header.numTiles; ++i) {
		NavMeshTileHeader tileHeader = *reinterpret_cast<NavMeshTileHeader*>(file_ptr);
		file_ptr += sizeof(NavMeshTileHeader);
		
		unsigned char* tdata = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		memcpy(tdata, file_ptr, tileHeader.dataSize);
		m_navMesh->addTile(tdata, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		file_ptr += tileHeader.dataSize;
	}

	m_navMeshQuery->init(m_navMesh, 2048);

	return false;
}

float randFunc() {
	std::uniform_real_distribution<std::float_t> dist(0.0f, 1.0f);
	return dist(RandomUtil::GetEngine());
}

DataTypes::Vector3 NavMeshManager::findRandomPointAroundCircle(DataTypes::Vector3 start, std::float_t maxDistance) {
	if (m_navMesh == nullptr) return start;

	dtQueryFilter m_filter;
	m_filter.setIncludeFlags(0xffff);
	m_filter.setExcludeFlags(0);

	float m_polyPickExt[3];
	m_polyPickExt[0] = 1;
	m_polyPickExt[1] = 1;
	m_polyPickExt[2] = 1;


	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	float nearestPt[3];

	
	float m_spos[3];
	m_spos[0] = start.x;
	m_spos[1] = start.y;
	m_spos[2] = start.z;

	dtStatus m_status = m_navMeshQuery->findNearestPoly(m_spos, m_polyPickExt, &m_filter, &m_startRef, nearestPt);
	
	dtPolyRef randomRef;
	float randomPt[3];
	m_navMeshQuery->findRandomPointAroundCircle(m_startRef, m_spos, maxDistance, &m_filter, randFunc, &randomRef, randomPt);
	m_status = m_navMeshQuery->findNearestPoly(randomPt, m_polyPickExt, &m_filter, &m_startRef, nearestPt);
	randomPt[1] = nearestPt[1];
	//randomPt[1] -= .5f;

	return DataTypes::Vector3(randomPt[0], randomPt[1], randomPt[2]);
}