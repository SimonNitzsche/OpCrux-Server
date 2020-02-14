set(GLAD_SRC
	file.cpp
	glad/glad.h
	KHR/khrplatform.h
)

add_library(glad STATIC ${GLAD_SRC})
