#include <iostream>
#include <GLFW/glfw3.h>

#define RAMIEL_ARGS_IMPLEMENTATION
#include <ramiel/args.h>
#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene() {
	graphics::loadTexture("brick_texture", "examples/assets/textures/brickwall_texture.jpg", 'c');
	graphics::loadTexture("brick_normal", "examples/assets/textures/brickwall_normal.jpg", 'n');
	graphics::loadMesh("cube", "examples/assets/models/cube.obj");

	// lighting
	graphics::setAmbientLightColor({ 25, 25, 25 });
	graphics::addPointLight(vec3f_255, 4.0f, { -6, 2, -2 }, 0.2);
	graphics::addPointLight(vec3f_255, 4.0f, {  0, 2, -2 }, 0.2);
	graphics::addPointLight(vec3f_255, 4.0f, {  6, 2, -2 }, 0.2);
	//graphics::addEffect(hdr);

	// flat
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ -9, 0, 0 } },
		{ "shading", ShadingType::FLAT },
		{ "color", Vec3f{ 100, 90, 70 } }
	});

	// flat textured
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ -7, 0, 0 } },
		{ "shading", ShadingType::FLAT },
		{ "texture", "brick_texture" }
	});

	// vertex
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ -5, 0, 0 } },
		{ "shading", ShadingType::VERTEX },
		{ "color", Vec3f{ 100, 90, 70 } }
	});

	// vertex textured
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ -3, 0, 0 } },
		{ "shading", ShadingType::VERTEX },
		{ "texture", "brick_texture" }
	});

	// pixel
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ -1, 0, 0 } },
		{ "shading", ShadingType::PIXEL },
		{ "color", Vec3f{ 100, 90, 70 } }
	});

	// pixel textured
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ 1, 0, 0 } },
		{ "shading", ShadingType::PIXEL },
		{ "texture", "brick_texture" }
	});

	// pixel normal mapped
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ 3, 0, 0 } },
		{ "shading", ShadingType::PIXEL },
		{ "color", Vec3f{ 100, 90, 70 } },
		{ "normalMap", "brick_normal" }
	});

	// pixel textured and normal mapped
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ 5, 0, 0 } },
		{ "shading", ShadingType::PIXEL },
		{ "texture", "brick_texture" },
		{ "normalMap", "brick_normal" }
	});

	// pixel smooth
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ 7, 0, 0 } },
		{ "shading", ShadingType::PIXEL_S },
		{ "color", Vec3f{ 100, 90, 70 } }
	});

	// pixel smooth textured
	graphics::addEntity({
		{ "mesh", "cube" },
		{ "pos", Vec3f{ 9, 0, 0 } },
		{ "shading", ShadingType::PIXEL_S },
		{ "texture", "brick_texture" }
	});
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
