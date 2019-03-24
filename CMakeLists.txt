cmake_minimum_required(VERSION 3.5)
project(LUReborn_Server_3_0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} #[[additional compiler flags:]])
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
	add_definitions(-D_USE_MATH_DEFINES)
endif()

add_subdirectory(libs)
include_directories(include)
include_directories(libs)
include_directories(src)
include_directories(tests)
include_directories(${MYSQL_INCLUDE_DIR})
include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIRS})

add_executable(LUReborn_Server_3_0
        src/Common/CrossPlatform.hpp
        src/DataTypes/AMF3.hpp
		src/DataTypes/LEGO_GUID.hpp
        src/DataTypes/LWOOBJID.hpp
		src/DataTypes/Position.hpp
		src/DataTypes/Quaternion.hpp
		src/DataTypes/Vector3.hpp
		src/DataTypes/ZoneInfo.hpp
		src/Entity/Components/Interface/IEntityComponent.hpp
		src/Entity/GameObject.cpp
		src/Entity/GameObject.hpp
        src/Enums/EComponents.hpp
        src/Enums/EGameMessages.hpp
		src/Enums/EGeneralPackets.hpp
        src/Enums/EPackets.hpp
		src/Enums/EPathBehavior.hpp
		src/Enums/EPathType.hpp
		src/Enums/EPropertyAchievement.hpp
		src/Enums/EPropertyRentalTimeUnit.hpp
        src/Enums/ERemoteConnection.hpp
		src/Enums/ESystem.hpp
		src/Enums/StringEnums.hpp
		src/Exceptions/ExNetException.hpp
		src/FileTypes/LUZFile/LUZone.cpp
		src/FileTypes/LUZFile/LUZone.hpp
		src/GameCache/Interface/FastDatabase.cpp
		src/GameCache/Interface/FastDatabase.hpp
		src/GameCache/BehaviorParameter.hpp
		src/GameCache/DestructibleComponent.hpp
		src/GameCache/Emotes.hpp
		src/GameCache/ItemComponent.hpp
		src/GameCache/LootMatrix.hpp
		src/GameCache/LootMatrixIndex.hpp
		src/GameCache/MovingPlatforms.hpp
		src/GameCache/Objects.hpp
		src/GameCache/PetAbilities.hpp
		src/GameCache/PetComponent.hpp
		src/GameCache/PhysicsComponent.hpp
		src/GameCache/Preconditions.hpp
		src/GameCache/PropertyEntranceComponent.hpp
		src/GameCache/ScriptComponent.hpp
		src/GameCache/SkillBehavior.hpp
		src/GameCache/VendorComponent.hpp
		src/GameCache/WorldConfig.hpp
		src/NetworkDataTypes/ByteBool.hpp
		src/NetworkDataTypes/LUPacket.hpp
		src/PacketFactory/Auth/AuthPackets.hpp
		src/PacketFactory/General/GeneralPackets.hpp
		src/PacketFactory/World/WorldPackets.hpp
		src/Replica/ReplicaTypes.hpp
		src/Server/Bridges/BridgeMasterServer.cpp
		src/Server/Bridges/BridgeMasterServer.hpp
		src/Server/Interfaces/ILUServer.hpp
		src/Server/Manager/WorldInstanceManager.hpp
		src/Server/AuthServer.cpp
		src/Server/AuthServer.hpp
		src/Server/MasterServer.cpp
		src/Server/MasterServer.hpp
		src/Server/WorldServer.cpp
		src/Server/WorldServer.hpp
		src/Sessions/ClientSession.hpp
		src/Sessions/SessionManager.hpp
		src/Structs/Networking/General/StructPacketHeader.hpp
		src/Utils/FileUtils.hpp
		src/Utils/Logger.cpp
		src/Utils/Logger.hpp
		src/Utils/PacketUtil.hpp
		src/Utils/ServerInfo.cpp
		src/Utils/ServerInfo.hpp
		src/Utils/StringUtils.hpp
		src/WebInterface/DashboardAPI.cpp
		src/WebInterface/DashboardAPI.hpp
		src/WebInterface/DashboardResources.hpp
		src/WebInterface/DashboardTools.hpp
		src/WebInterface/WebInterface.hpp
		src/Main.cpp
)

set_property(TARGET json PROPERTY CXX_STANDARD 98)
set_property(TARGET Mongoose PROPERTY CXX_STANDARD 98)
set_property(TARGET RakNet PROPERTY CXX_STANDARD 98)

target_link_libraries(LUReborn_Server_3_0
		json
		Mongoose
		RakNet
)
link_directories(${MYSQL_LIBRARIES})
link_directories(${MYSQLCONNECTORCPP_LIBRARIES})

if(WIN32)
	target_link_libraries(LUReborn_Server_3_0 wsock32 ws2_32)
endif()
if(UNIX)
	target_link_libraries(LUReborn_Server_3_0 pthread)
endif()
