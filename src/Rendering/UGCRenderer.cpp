#include "Rendering/UGCRenderer.hpp"

#include "Utils/Logger.hpp"

UGCRenderer::UGCRenderer() {
	window = glfwCreateWindow(800, 600, "UGC Renderer", nullptr, nullptr);

	if (window == nullptr) {
		Logger::log("UGCR", "Unable to create UGC Renderer", LogType::ERR);
		glfwTerminate();
	}
	else {
		glfwMakeContextCurrent(window);

		// C-Cast somehow ¯\_(ツ)_/¯
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			Logger::log("UGCR", "Unable to initialize GLAD", LogType::ERR);
		}
		else {
			glViewport(0, 0, 800, 600);
		}
	}
}

void UGCRenderer::Paint() {
	glfwMakeContextCurrent(window);
	if (glfwWindowShouldClose(window)) {
		glfwTerminate();
		return;
	}

	glfwSwapBuffers(window);
	glfwPollEvents();

	
}
