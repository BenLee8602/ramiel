#include <iostream>
#include <GLFW/glfw3.h>

#define RAMIEL_ARGS_IMPLEMENTATION
#include <ramiel/args.h>
#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene() {
	graphics::loadMesh("sphere", "examples/assets/models/sphere.obj");
	graphics::setAmbientLightColor({ 100, 100, 100 });
	graphics::addDirLight();

	//*
	graphics::addEntity({
		{ "mesh", "sphere" },
		{ "color", Vec3f{ 255, 25, 25 } },
		{ "pos", Vec3f{ 0, 0, 32 } },
		{ "colliderType", ColliderType::SPHERE },
		{ "hbxrad", 1.0f }
	});
	graphics::addEntity({
		{ "mesh", std::string("sphere") },
		{ "color", Vec3f{ 25, 255, 25 } },
		{ "pos", Vec3f{ 0, 4, 32 } },
		{ "dynamic", true },
		{ "posAcc", Vec3f{ 0, -9.8, 0 } },
		{ "colliderType", ColliderType::SPHERE },
		{ "hbxrad", 1.0f }
	});
	graphics::addEntity({
		{ "mesh", std::string("sphere") },
		{ "color", Vec3f{ 25, 25, 255 } },
		{ "pos", Vec3f{ 0.5, 16, 32 } },
		{ "dynamic", true },
		{ "posAcc", Vec3f{ 0, -9.8, 0 } },
		{ "colliderType", ColliderType::SPHERE },
		{ "hbxrad", 1.0f }
	});
	//*/

	/*
	graphics::addEntity({
		{ "mesh", std::string("sphere") },
		{ "color", Vec3f{ 255, 25, 255 } },
		{ "pos", Vec3f{ -8, 0, 32 } },
		{ "posVel", Vec3f{ 2, 0, 0 } },
		{ "dynamic", true },
		{ "colliderType", ColliderType::SPHERE },
		{ "hbxrad", 1.0f }
	});
	graphics::addEntity({
		{ "mesh", std::string("sphere") },
		{ "color", Vec3f{ 25, 255, 255 } },
		{ "pos", Vec3f{ -2, 0, 32 } },
		{ "posVel", Vec3f{ 1, 0, 0 } },
		{ "dynamic", true },
		{ "colliderType", ColliderType::SPHERE },
		{ "hbxrad", 1.0f },
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
