add_subdirectory(reactphysics3d)
add_subdirectory(json)
add_subdirectory(libbcrypt/include/bcrypt)
add_subdirectory(Mongoose)
add_subdirectory(odbc-cpp-wrapper)
add_subdirectory(RakNet)
add_subdirectory(random)
add_subdirectory(recastnavigation)
add_subdirectory(TinyXML2)

set_property(TARGET json PROPERTY CXX_STANDARD 98)
set_property(TARGET Mongoose PROPERTY CXX_STANDARD 98)
set_property(TARGET RakNet PROPERTY CXX_STANDARD 98)
set_property(TARGET TinyXML2 PROPERTY CXX_STANDARD 98)

add_definitions(-DODBC_STATIC)
add_definitions(-Wno-dev)

find_package(ODBC REQUIRED)
