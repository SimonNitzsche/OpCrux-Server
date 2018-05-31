cmake_minimum_required(VERSION 3.5)
project(LUReborn_Server_3_0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} #[[additional compiler flags:]])
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

include_directories(libs)
include_directories(src)
add_subdirectory(libs)

add_executable(LUReborn_Server_3_0
        src/Common/CrossPlatform.hpp
        src/DataTypes/AMF3.hpp
        src/DataTypes/LWOOBJID.hpp
		src/DataTypes/Position.hpp
		src/DataTypes/Vector3.hpp
        src/Enums/EComponents.hpp
        src/Enums/EGameMessages.hpp
        src/Enums/EPackets.hpp
        src/Enums/ERemoteConnection.hpp
		src/Enums/StringEnums.hpp
        src/Server/Bridges/BridgeMasterServer.cpp
        src/Server/Bridges/BridgeMasterServer.hpp
        src/Server/Interfaces/ILUServer.hpp
        src/Server/AuthServer.cpp
        src/Server/AuthServer.hpp
        src/Server/MasterServer.cpp
        src/Server/MasterServer.hpp
        src/Server/WorldServer.cpp
        src/Server/WorldServer.hpp
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

target_link_libraries(LUReborn_Server_3_0
		json
		Mongoose
		RakNet
)

if(WIN32)
    target_link_libraries(LUReborn_Server_3_0 wsock32 ws2_32)
endif()
if(UNIX)
    target_link_libraries(LUReborn_Server_3_0 pthread)
endif()
