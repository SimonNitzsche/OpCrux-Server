#ifndef __MISC__NAVMESHMANAGER_HPP__
#define __MISC__NAVMESHMANAGER_HPP__

#include <memory>
#include <string>
#include "../../libs/recastnavigation/Detour/Include/DetourNavMeshQuery.h"
#include "DataTypes/Vector3.hpp"

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader
{
	char magic[4];
	int version;
	int numTiles;
	dtNavMeshParams params;
};

struct NavMeshTileHeader
{
	dtTileRef tileRef;
	int dataSize;
};

class NavMeshManager {
private:
	std::unique_ptr<dtNavMesh> m_navMeshOwner;
	dtNavMesh* m_navMesh;
	std::unique_ptr<dtNavMeshQuery> m_navMeshQueryOwner;
	dtNavMeshQuery* m_navMeshQuery;
	NavMeshSetHeader header;
public:
	bool Initialise();
	bool Release();
	bool Open(std::string filepath);
	DataTypes::Vector3 findRandomPointAroundCircle(DataTypes::Vector3 start, std::float_t maxDistance);
	//bool AddTile(const char* fn);
};

#endif