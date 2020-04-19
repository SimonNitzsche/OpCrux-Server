#include "Rendering/DebugRenderer.hpp"

#include "Utils/Logger.hpp"

#include "Server/WorldServer.hpp"


void DebugRenderer::AssignZoneInstance(WorldServer* worldServer) {
	wsInstance = worldServer;
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

#include "FileTypes/GFile/GeometryFile.hpp"

DebugRenderer::DebugRenderer() {
	//window = glfwCreateWindow(800, 600, "Debug Window Renderer", nullptr, nullptr);

	//if (window == nullptr) {
	//	Logger::log("WDBG", "Unable to create Debug Renderer", LogType::ERR);
	//	glfwTerminate();
	//}
	//else {
	//	glfwMakeContextCurrent(window);

	//	// C-Cast somehow ¯\_(ツ)_/¯
	//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//		Logger::log("WDBG", "Unable to initialize GLAD", LogType::ERR);
	//	}
	//	else {
	//		glViewport(0, 0, 800, 600);


 //           // build and compile our shader program
 //  // ------------------------------------
 //  // vertex shader
 //           int vertexShader = glCreateShader(GL_VERTEX_SHADER);
 //           glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 //           glCompileShader(vertexShader);
 //           // check for shader compile errors
 //           int success;
 //           char infoLog[512];
 //           glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
 //           if (!success)
 //           {
 //               glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
 //               std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
 //           }
 //           // fragment shader
 //           int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 //           glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 //           glCompileShader(fragmentShader);
 //           // check for shader compile errors
 //           glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
 //           if (!success)
 //           {
 //               glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
 //               std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
 //           }
 //           // link shaders
 //           shaderProgram = glCreateProgram();
 //           glAttachShader(shaderProgram, vertexShader);
 //           glAttachShader(shaderProgram, fragmentShader);
 //           glLinkProgram(shaderProgram);
 //           // check for linking errors
 //           glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 //           if (!success) {
 //               glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
 //               std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
 //           }
 //           glDeleteShader(vertexShader);
 //           glDeleteShader(fragmentShader);

 //           // set up vertex data (and buffer(s)) and configure vertex attributes
 //           // ------------------------------------------------------------------

 //           std::string gName = "E:\\Games\\LU\\LEGO Universe (unpacked)\\res\\brickprimitives\\lod0\\30152.g";
 //           GeometryFile geoFile = GeometryFile(gName.c_str());
 //           
 //           

 //           float vertices[] = {
 //               -0.5f, -0.5f, 0.0f, // left  
 //                0.5f, -0.5f, 0.0f, // right 
 //                0.0f,  0.5f, 0.0f  // top   
 //           };

 //           glGenVertexArrays(1, &VAO);
 //           glGenBuffers(1, &VBO);
 //           // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
 //           glBindVertexArray(VAO);

 //           glBindBuffer(GL_ARRAY_BUFFER, VBO);
 //           //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 //           
 //           std::vector<float> verts;
 //           GnV = *geoFile.vertexCount;
 //           GnI = *geoFile.indexCount;
 //           for (int i = 0; i < *geoFile.vertexCount; ++i) {
 //               verts.push_back((geoFile.vertexCoordinates + i)->x);
 //               verts.push_back((geoFile.vertexCoordinates + i)->y);
 //               verts.push_back((geoFile.vertexCoordinates + i)->z);
 //           }

 //           glBufferData(GL_ARRAY_BUFFER, sizeof(*geoFile.vertexCount) * 3, geoFile.vertexCoordinates, GL_STATIC_DRAW);


 //           glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 //           glEnableVertexAttribArray(0);

 //           glGenBuffers(1, &EBO);
 //           glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 //           glBufferData(GL_ELEMENT_ARRAY_BUFFER, *geoFile.indexCount * sizeof(uint32_t), geoFile.triangles, GL_STATIC_DRAW);

 //           // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
 //           glBindBuffer(GL_ARRAY_BUFFER, 0);

 //           // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
 //           // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
 //           glBindVertexArray(0);
	//	}
	//}
}

void DebugRenderer::Paint() {
//	if (glfwWindowShouldClose(window)) {
//		glfwTerminate();
//		return;
//	}
//
//
//	
//    // render
//       // ------
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    // draw our first triangle
//    glUseProgram(shaderProgram);
//    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
//    glDrawArrays(GL_TRIANGLES, 0, GnV);
//    //glDrawElements(GL_TRIANGLES, GnI, GL_UNSIGNED_INT, 0);
//    // glBindVertexArray(0); // no need to unbind it every time 
//
//
//	//if (wsInstance == nullptr) return;
//	//glfwMakeContextCurrent(window);
//
//	glClear(GL_COLOR_BUFFER_BIT);
//	glfwSwapBuffers(window);
//	glfwPollEvents();
//
//
}
