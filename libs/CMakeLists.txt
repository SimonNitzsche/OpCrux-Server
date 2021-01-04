add_subdirectory(bullet3-2.89)
add_subdirectory(glad/include)
add_subdirectory(glfw-3.3.2)
add_subdirectory(json)
add_subdirectory(libbcrypt/include/bcrypt)
add_subdirectory(Mongoose)
add_subdirectory(odbc-cpp-wrapper)
add_subdirectory(RakNet)
add_subdirectory(random)
add_subdirectory(SDL/include)
add_subdirectory(recastnavigation)
add_subdirectory(TinyXML2)

set_property(TARGET glad PROPERTY CXX_STANDARD 98)
set_property(TARGET json PROPERTY CXX_STANDARD 98)
set_property(TARGET Mongoose PROPERTY CXX_STANDARD 98)
set_property(TARGET RakNet PROPERTY CXX_STANDARD 98)
set_property(TARGET rapidxml PROPERTY CXX_STANDARD 98)

add_definitions(-DODBC_STATIC)
add_definitions(-Wno-dev)

set(GLFW_BUILD_DOCS FALSE)

#some code to support OpenGL and Glew cross platform
IF (WIN32)
        INCLUDE_DIRECTORIES(
             ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
        )
        ADD_DEFINITIONS(-DGLEW_STATIC)
	LINK_LIBRARIES( ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
ELSE(WIN32)
        IF(APPLE)
                find_library(COCOA NAMES Cocoa)
                MESSAGE(${COCOA})
                link_libraries(${COCOA} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})

        ELSE(APPLE)
                INCLUDE_DIRECTORIES(
                        ${BULLET_PHYSICS_SOURCE_DIR}/btgui/OpenGLWindow/GlewWindows
                )
                ADD_DEFINITIONS("-DGLEW_INIT_OPENGL11_FUNCTIONS=1")
                ADD_DEFINITIONS("-DGLEW_STATIC")
                ADD_DEFINITIONS("-DGLEW_DYNAMIC_LOAD_ALL_GLX_FUNCTIONS=1")

                LINK_LIBRARIES(  pthread ${DL} )
        ENDIF(APPLE)
ENDIF(WIN32)

find_package(ODBC REQUIRED)