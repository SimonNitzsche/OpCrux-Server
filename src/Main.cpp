#define HOST_ENDIAN_IS_BIG
#define BIG_ENDIAN

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>

#include "Common/CrossPlatform.hpp"

#include <RakNet/RakSleep.h>

#include "Server/MasterServer.hpp"
#include "Server/AuthServer.hpp"
#include "Server/WorldServer.hpp"
#include "Server/Bridges/BridgeMasterServer.hpp"
#include "WebInterface/WebInterface.hpp"

#include "DataTypes/AMF3.hpp"

#include "Utils/ServerInfo.hpp"
#include <chrono>
using namespace std::chrono;

std::vector<ILUServer *> virtualServerInstances;

enum class SERVERMODE : uint8_t { STANDALONE, MASTER, WORLD, AUTH } MODE_SERVER;

// Following Includes are for testing
#include "FileTypes/LUZFile/LUZone.hpp"
#include "Database/FastDatabase.hpp"

int main(int argc, char* argv[]) {
	std::string ipMaster = "127.0.0.1";

	//LUZone("./res/maps/01_live_maps/avant_gardens/nd_avant_gardens.luz");
	FDB::Connection conn = FDB::Connection("./res/cdclient.fdb");
	/*uint32_t tc = conn.getTableCount();
	for (int i = 0; i < tc; ++i) {
		Logger::log("TEST", conn.getTableHeader().getColumnHeader(i).getTableName());
		uint32_t cc = conn.getTableHeader().getColumnHeader(i).getColumnCount();
		for (int j = 0; j < cc; ++j) {
			Logger::log("TEST", "    " + (std::string)conn.getTableHeader().getColumnHeader(i).getColumnData().getColumnName(j)
				+ " " + std::to_string((uint32_t)conn.getTableHeader().getColumnHeader(i).getColumnData().getColumnDataType(j)));
		}
		
		uint32_t rc = conn.getTableHeader().getRowTopHeader(i).getRowCount();
		for (int k = 0; k < rc; ++k) {
			if (conn.getTableHeader().getRowTopHeader(i).getRowHeader().isRowInfoValid(k)) {
				FDB::RowInfo rI = conn.getTableHeader().getRowTopHeader(i).getRowHeader().getRowInfo(k);
				while (true) {
					for (int j = 0; j < cc; ++j) {
						if (!rI.getRowDataHeader().isRowDataValid()) continue;
						Logger::log("TEST", rI.getRowDataHeader().getRowData().getColumnData(j), LogType::PASSED);
					}
					if (rI.isLinkedRowInfoValid())
						rI = rI.getLinkedRowInfo();
					else
						break;
				}
			}
		}
		Logger::log("TEST", "        Row Count: " + std::to_string(conn.getTableHeader().getRowTopHeader(i).getRowCount()), LogType::INFO);
	}*/

	//int index = conn.getTableIndex();
	//std::unordered_map<FDB::PointerString, FDB::PointerString>
	/*FDB::RowTopHeader rows = conn.getRows("ZoneTable");
	FDB::RowInfo row0 = rows[0];
	std::string row0data = row0["zoneName"];*/
	uint64_t total_fields = 0;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	uint32_t tblCount = conn.getTableCount();
	for (int table_index = 0; table_index < tblCount; ++table_index) {
		FDB::QueryResult qr = conn.Query(table_index, [](std::string col, std::string val) -> bool { return true; });
		for each(FDB::RowInfo ri in qr) {
			FDB::RowInfo cri = ri;
			bool exec = false;
			while (cri.isLinkedRowInfoValid() || !exec) {
				if (!exec) exec = true;
				for (int i = 0; i < cri.getColumnCount(); ++i) {
					//Logger::log(cri.getColumnName(i), (FDB::FieldValue)cri[i]);
					(FDB::FieldValue)cri[i];
					++total_fields;
				}
				if (cri.isLinkedRowInfoValid())
					cri = cri.getLinkedRowInfo();
				else
					break;
			}
		}
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "Going through all tables, rows and fields ( "<<total_fields<<" ): " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds." << std::endl;
	std::cout << "Time consumed for each field: " << (std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() / total_fields) << " nanoseconds." << std::endl;
	

	MODE_SERVER = SERVERMODE::STANDALONE;
#ifdef WIN32
	std::system("title LUReborn Server 3.0 (Standalone)");
#endif
	for (int i = 0; i < argc; i++) {
		std::string arg = std::string(argv[i]);
		if (arg == "--master") {
			MODE_SERVER = SERVERMODE::MASTER;
#ifdef WIN32
			std::system("title LUReborn Server 3.0 (Master only)");
#endif
		}
		if (arg == "--world") {
			MODE_SERVER = SERVERMODE::WORLD;
#ifdef WIN32
			std::system("title LUReborn Server 3.0 (World only)");
#endif
		}
		if (arg == "--auth") {
			MODE_SERVER = SERVERMODE::AUTH;
#ifdef WIN32
			std::system("title LUReborn Server 3.0 (Auth only)");
#endif
		}
		else {
			continue;
		}

		if (argc >= 3) {
			ipMaster = argv[i + 1];
		}

		break;
	}

	ServerInfo::init();

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::MASTER) {
		std::thread mT([](MasterServer * ms) { ms = new MasterServer(); }, ServerInfo::masterServer);
		mT.detach();

		std::thread wiT([]() { WebInterface::WebInterfaceLoop(); });
		wiT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER != SERVERMODE::MASTER) {
		BridgeMasterServer* masterServerBridge = new BridgeMasterServer(ipMaster);
		masterServerBridge->Connect();
		masterServerBridge->Listen();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::AUTH) {
		AuthServer * aS;
		std::thread aT([](AuthServer * as) { as = new AuthServer(); }, aS);
		aT.detach();
	}

	if (MODE_SERVER == SERVERMODE::STANDALONE || MODE_SERVER == SERVERMODE::WORLD) {
		//std::thread wT([]() { new WorldServer(); });
		//wT.detach();
	}

	while (ServerInfo::bRunning) RakSleep(30);

	std::system("pause");

}
