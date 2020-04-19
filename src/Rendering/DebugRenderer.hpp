#ifndef __RENDERING__DEBUGRENDERER_HPP__
#define __RENDERING__DEBUGRENDERER_HPP__


class WorldServer;
class GLFWwindow;

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