set(GLAD_SRC
	EGL/eglplatform.h
	glad/egl.h
	glad/gl.h
	glad/glx.h
	KHR/khrplatform.h
	egl.c
	gl.c
	glx.c
	linmath.c
)

add_library(glad STATIC ${GLAD_SRC})
