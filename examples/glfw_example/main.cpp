#include <GLFW/glfw3.h>

#include <ramiel/core.h>
#include <ramiel/shaders.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


int main() {
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(width, height, "ramiel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	Scene scene;
    scene.loadMesh<Vertex_Mesh>("examples/assets/models/cube.obj", "cube");
    scene.addEntity<Vertex_Mesh>(
        "cube",
        VS_PerPixel(scene.camera),
        PS_PerPixel(scene.getLightingList(1, 0.0f), vec3f_255)
    );
    scene.camera.res({ width, height });
	scene.camera.pos = { 1.0f, 1.0f, -4.0f };
	scene.ambientLight = { 25.0f, 25.0f, 25.0f };
	scene.addLight(new Light_Pt(vec3f_255, 1.0f, { 1.0f, 1.0f, -4.0f }, 0.1f));

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
