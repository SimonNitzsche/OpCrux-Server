# - Try to find MySQL C and C++ Connector librarys
# This module defines
#  MYSQL_FOUND
#  MYSQL_INCLUDE_DIR, where to find mysql.h
#  MYSQL_LIBRARIES, the libraries needed to use MySQL.
#  MYSQLCONNECTORCPP_FOUND
#  MYSQLCONNECTORCPP_INCLUDE_DIRS - the Mysql-Connector-C++ include directories
#  MYSQLCONNECTORCPP_LIBRARIES - link these to use Mysql-Connector-C++

if(WIN32)
    find_path(MYSQL_INCLUDE_DIR mysql.h
        PATHS
        $ENV{MYSQL_INCLUDE_DIR}
        $ENV{MYSQL_DIR}/include
        $ENV{ProgramFiles}/MySQL/*/include
        $ENV{SystemDrive}/MySQL/*/include
        $ENV{ProgramW6432}/MySQL/*/include
    )
else(WIN32)
    find_path(MYSQL_INCLUDE_DIR mysql.h
        PATHS
        $ENV{MYSQL_INCLUDE_DIR}
        $ENV{MYSQL_DIR}/include
        /usr/local/mysql/include
        /opt/mysql/mysql/include
        PATH_SUFFIXES
        mysql
    )
endif(WIN32)

if(WIN32)
    if (${CMAKE_BUILD_TYPE})
        string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_TOLOWER)
    endif()

    # path suffix for debug/release mode
    # binary_dist: mysql binary distribution
    # build_dist: custom build
    if(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
        set(binary_dist debug)
        set(build_dist Debug)
    else(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")
        ADD_DEFINITIONS(-DDBUG_OFF)
        set(binary_dist opt)
        set(build_dist Release)
    endif(CMAKE_BUILD_TYPE_TOLOWER MATCHES "debug")

    #   find_library(MYSQL_LIBRARIES NAMES mysqlclient
    set(MYSQL_LIB_PATHS
        $ENV{MYSQL_DIR}/lib/${binary_dist}
        $ENV{MYSQL_DIR}/libmysql/${build_dist}
        $ENV{MYSQL_DIR}/client/${build_dist}
        $ENV{ProgramFiles}/MySQL/*/lib/${binary_dist}
        $ENV{SystemDrive}/MySQL/*/lib/${binary_dist}
        $ENV{MYSQL_DIR}/lib/opt
        $ENV{MYSQL_DIR}/client/release
        $ENV{ProgramFiles}/MySQL/*/lib/opt
        $ENV{SystemDrive}/MySQL/*/lib/opt
        $ENV{ProgramW6432}/MySQL/*/lib
    )
    find_library(MYSQL_LIBRARIES NAMES libmysql
        PATHS
        ${MYSQL_LIB_PATHS}
    )
else(WIN32)
    #   find_library(MYSQL_LIBRARIES NAMES mysqlclient
    set(MYSQL_LIB_PATHS
        $ENV{MYSQL_DIR}/libmysql_r/.libs
        $ENV{MYSQL_DIR}/lib
        $ENV{MYSQL_DIR}/lib/mysql
        /usr/local/mysql/lib
        /opt/mysql/mysql/lib
        $ENV{MYSQL_DIR}/libmysql_r/.libs
        $ENV{MYSQL_DIR}/lib
        $ENV{MYSQL_DIR}/lib/mysql
        /usr/local/mysql/lib
        /opt/mysql/mysql/lib
        PATH_SUFFIXES
        mysql
    )
    find_library(MYSQL_LIBRARIES NAMES mysqlclient
        PATHS
        ${MYSQL_LIB_PATHS}
    )
endif(WIN32)

set(CMAKE_REQUIRED_INCLUDES ${MYSQL_INCLUDE_DIR})

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
    set(MYSQL_FOUND TRUE)
    message(STATUS "Found MySQL: ${MYSQL_INCLUDE_DIR}, ${MYSQL_LIBRARIES}")
else(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)
    set(MYSQL_FOUND FALSE)
    message(STATUS "MySQL not found.")
endif(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARIES)

mark_as_advanced(MYSQL_INCLUDE_DIR MYSQL_LIBRARIES)

set(MYSQLCONNECTORCPP_ROOT_DIR
    "${MYSQLCONNECTORCPP_ROOT_DIR}"
    CACHE
    PATH
    "additional search paths"
)

if(WIN32)
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR
        NAMES
        mysql_connection.h
        PATHS
        "C:\\Program Files"
        HINTS
        ${MYSQLCONNECTORCPP_ROOT_DIR}
        PATH_SUFFIXES
        include
    )

    find_library(MYSQLCONNECTORCPP_LIBRARY
        NAMES
        mysqlcppconn
        mysqlcppconn-static
        HINTS
        ${MYSQLCONNECTORCPP_ROOT_DIR}
        PATH_SUFFIXES
        lib
    )

else(WIN32)
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR
        mysql_connection.h
        HINTS
        ${MYSQLCONNECTORCPP_ROOT_DIR}
        PATH_SUFFIXES
        include
    )

    find_library(MYSQLCONNECTORCPP_LIBRARY
        NAMES
        mysqlcppconn
        mysqlcppconn-static
        HINTS
        ${MYSQLCONNECTORCPP_ROOT_DIR}
        PATH_SUFFIXES
        lib64
        lib
    )
endif()

mark_as_advanced(MYSQLCONNECTORCPP_INCLUDE_DIR MYSQLCONNECTORCPP_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MysqlConnectorCpp
    DEFAULT_MSG
    MYSQLCONNECTORCPP_INCLUDE_DIR
    MYSQLCONNECTORCPP_LIBRARY
)

if(MYSQLCONNECTORCPP_FOUND)
    set(MYSQLCONNECTORCPP_INCLUDE_DIRS "${MYSQLCONNECTORCPP_INCLUDE_DIR}")
    set(MYSQLCONNECTORCPP_LIBRARIES "${MYSQLCONNECTORCPP_LIBRARY}")
    mark_as_advanced(MYSQLCONNECTORCPP_ROOT_DIR)
    message(STATUS "Found MySQLConnectorCPP: ${MYSQLCONNECTORCPP_INCLUDE_DIRS}, ${MYSQLCONNECTORCPP_LIBRARIES}")
endif()
