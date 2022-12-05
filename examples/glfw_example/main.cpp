#include <iostream>
#include <GLFW/glfw3.h>

#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene(Scene& scene) {
	scene.loadMesh("../assets/models/cube.obj", "cube");
}


int main() {
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(width, height, "ramiel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	Scene scene;
	scene.camera.setBufferSize({ width, height });
	initScene(scene);

	uint8_t* frame = new uint8_t[width * height * 3];

	glfwSetTime(0);
	double frameStart = glfwGetTime();
	double frameEnd = glfwGetTime();
	double dtime = frameEnd - frameStart;
	
	while (!glfwWindowShouldClose(window)) {
		frameStart = glfwGetTime();

		scene.renderFrame();
		scene.camera.getFrameRGB(frame);

		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frame);
		glfwSwapBuffers(window);
		glfwPollEvents();

		frameEnd = glfwGetTime();
		dtime = frameEnd - frameStart;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	delete[] frame;
	return 0;
}
