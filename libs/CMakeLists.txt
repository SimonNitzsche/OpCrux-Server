add_subdirectory(json)
add_subdirectory(Mongoose)
add_subdirectory(RakNet)

# MySQL Library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/libs/")
find_package(MySQL REQUIRED)

if(MYSQL_FOUND)
    include_directories(${MYSQL_INCLUDE_DIR})
    link_directories(${MYSQLCONNECTORCPP_LIBRARIES})
endif()

if(MYSQLCONNECTORCPP_FOUND)
    include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIRS})
    link_directories(${MYSQL_LIB_DIR})
endif()
