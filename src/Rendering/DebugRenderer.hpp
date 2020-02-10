#ifndef __RENDERING__DEBUGRENDERER_HPP__
#define __RENDERING__DEBUGRENDERER_HPP__

#include <glad/include/glad/glad.h>
#include <GLFW/glfw3.h>

class WorldServer;

class DebugRenderer {
	GLFWwindow* window;
	WorldServer* wsInstance;
	int shaderProgram;
	unsigned int VBO, VAO, EBO;
	unsigned int GnV, GnI; 

public:
	void AssignZoneInstance(WorldServer* worldServer);
	DebugRenderer();
	void Paint();
};

#include "Server/WorldServer.hpp"

#endif