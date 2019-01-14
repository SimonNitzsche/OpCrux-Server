add_subdirectory(json)
add_subdirectory(Mongoose)
add_subdirectory(RakNet)

# MySQL Library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/libs/")
find_package(MySQL REQUIRED)

if(NOT MYSQLCONNECTORCPP_FOUND OR NOT MYSQL_FOUND)
    message(FATAL_ERROR "MySQL not found!")
endif()
