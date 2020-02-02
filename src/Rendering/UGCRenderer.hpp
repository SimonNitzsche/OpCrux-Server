#ifndef __RENDERING__UGCRENDERER_HPP__
#define __RENDERING__UGCRENDERER_HPP__

#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

class UGCRenderer {
	GLFWwindow* window;

public:
	UGCRenderer();
	void Paint();
};

#endif