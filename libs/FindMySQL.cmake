# - Try to find MySQL C and C++ Connector librarys
# This module defines
#  MYSQL_FOUND
#  MYSQL_INCLUDE_DIR, where to find mysql.h
#  MYSQL_LIBRARIES, the libraries needed to use MySQL.
#  MYSQLCONNECTORCPP_FOUND
#  MYSQLCONNECTORCPP_INCLUDE_DIR - the Mysql-Connector-C++ include directories
#  MYSQLCONNECTORCPP_LIBRARIES - link these to use Mysql-Connector-C++

if(WIN32)
    find_path(MYSQL_INCLUDE_DIR mysql.h
        PATHS
        $ENV{ProgramFiles}/MySQL/*/include
        $ENV{SystemDrive}/MySQL/*/include
        $ENV{ProgramW6432}/MySQL/*/include
    )
    if (${CMAKE_BUILD_TYPE})
        string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
    endif()

    if(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
        set(binary_dist debug)
        set(build_dist Debug)
    else(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
        ADD_DEFINITIONS(-DDBUG_OFF)
        set(binary_dist opt)
        set(build_dist Release)
    endif(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")

    find_library(MYSQL_LIBRARIES libmysql
        PATHS
        $ENV{ProgramFiles}/MySQL/*/lib/${binary_dist}
        $ENV{SystemDrive}/MySQL/*/lib/${binary_dist}
        $ENV{ProgramFiles}/MySQL/*/lib/opt
        $ENV{SystemDrive}/MySQL/*/lib/opt
        $ENV{ProgramW6432}/MySQL/*/lib
    )
else(WIN32)
    find_path(MYSQL_INCLUDE_DIR mysql.h
        PATHS
        /usr/local/mysql/include
        /usr/include/mysql
        /opt/mysql/mysql/include
    )
    find_library(MYSQL_LIBRARIES NAMES mysqlclient
        PATHS
        /usr/local/mysql/lib
        /opt/mysql/mysql/lib
    )
endif(WIN32)

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
    set(MYSQL_FOUND TRUE)
    message(STATUS "Found MySQL: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
    set(MYSQL_FOUND FALSE)
    message(STATUS "MySQL not found.")
endif(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARIES)

if(WIN32)
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR mysql_connection.h
        PATHS
		$ENV{ProgramFiles}/MySQL/*/include/jdbc
        $ENV{SystemDrive}/MySQL/*/include/jdbc
        $ENV{ProgramW6432}/MySQL/*/include/jdbc
    )
    find_library(MYSQLCONNECTORCPP_LIBRARIES mysqlcppconn
        PATHS
		$ENV{ProgramFiles}/MySQL/*/lib/vs14
        $ENV{SystemDrive}/MySQL/*/lib/vs14
        $ENV{ProgramW6432}/MySQL/*/lib/vs14
    )
else(WIN32)
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR mysql_connection.h
        PATHS
        /usr/include
    )
    find_library(MYSQLCONNECTORCPP_LIBRARIES mysqlcppconn
        PATHS
        /usr/lib
    )
endif()

mark_as_advanced(MYSQLCONNECTORCPP_INCLUDE_DIR MYSQLCONNECTORCPP_LIBRARIES)

if(MYSQLCONNECTORCPP_INCLUDE_DIR AND MYSQLCONNECTORCPP_LIBRARIES)
    set(MYSQLCONNECTORCPP_FOUND TRUE)
    message(STATUS "Found MySQLCppConnector: ${MYSQLCONNECTORCPP_INCLUDE_DIR}, ${MYSQLCONNECTORCPP_LIBRARIES}")
else(MYSQLCONNECTORCPP_INCLUDE_DIR AND MYSQLCONNECTORCPP_LIBRARIES)
    set(MYSQLCONNECTORCPP_FOUND FALSE)
    message(STATUS "MySQLCppConnector not found.")
endif(MYSQLCONNECTORCPP_INCLUDE_DIR AND MYSQLCONNECTORCPP_LIBRARIES)
