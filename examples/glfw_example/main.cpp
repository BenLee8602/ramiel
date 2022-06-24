#include <iostream>
#include <GLFW/glfw3.h>

#define RAMIEL_ARGS_IMPLEMENTATION
#include <ramiel/args.h>
#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene() {
	graphics::loadMesh("cube", "examples/assets/models/cube.obj");
	graphics::setAmbientLightColor({ 25, 25, 25 });
	graphics::addDirLight();

	//*
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "color", Vec3f{ 25, 25, 255 } },
		{ "pos", Vec3f{ 5, 4, 8 } },
		{ "colliderType", ColliderType::AABB },
		{ "dynamic", true },
		{ "posVel", Vec3f{ -1, -1, 0 } }
	});
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "color", Vec3f{ 255, 25, 25 } },
		{ "pos", Vec3f{ -2, 0, 8 } },
		{ "colliderType", ColliderType::AABB },
	});
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "color", Vec3f{ 25, 255, 25 } },
		{ "pos", Vec3f{ 0, -2, 8 } },
		{ "colliderType", ColliderType::AABB },
	});
	//*/
}


void getControls(GLFWwindow* window, bool controls[12]) {
	controls[0]  = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	controls[1]  = glfwGetKey(window, GLFW_KEY_Q);
	controls[2]  = glfwGetKey(window, GLFW_KEY_A);
	controls[3]  = glfwGetKey(window, GLFW_KEY_D);
	controls[4]  = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
	controls[5]  = glfwGetKey(window, GLFW_KEY_SPACE);
	controls[6]  = glfwGetKey(window, GLFW_KEY_S);
	controls[7]  = glfwGetKey(window, GLFW_KEY_W);
	controls[8]  = glfwGetKey(window, GLFW_KEY_RIGHT);
	controls[9]  = glfwGetKey(window, GLFW_KEY_LEFT);
	controls[10] = glfwGetKey(window, GLFW_KEY_DOWN);
	controls[11] = glfwGetKey(window, GLFW_KEY_UP);
}


int main() {
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(width, height, "ramiel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	graphics::setBufferSize({ width, height });
	initScene();

	uint8_t* frame = new uint8_t[width * height * 3];
	bool controls[12];

	glfwSetTime(0);
	double frameStart = glfwGetTime();
	double frameEnd = glfwGetTime();
	double dtime = frameEnd - frameStart;
	
	while (!glfwWindowShouldClose(window)) {
		frameStart = glfwGetTime();
		
		getControls(window, controls);
		graphics::setControls(controls);
		physics::simulatePhysics(dtime);
		graphics::renderFrame();
		graphics::getFrameRGB(frame);

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
