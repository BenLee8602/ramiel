#include <iostream>
#include <GLFW/glfw3.h>
#include <ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene() {
	/*
	graphics::loadModel("terrain", "examples/assets/models/terrain.obj", { -64, 0, -64 });
	graphics::setAmbientLightColor({ 100, 80, 100 });
	graphics::setBackgroundColor({ 150, 110, 110 });
	graphics::addDirLight({ 155, 40, 0 }, { -10, 1, 0 });
	graphics::addEntity("terrain", vec3f_255, ShadingType::PIXEL);
	graphics::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
	//*/

	//*
	graphics::loadModel("cube", "examples/assets/models/cube.obj");
	graphics::setAmbientLightColor({ 25, 25, 25 });
	graphics::addEntity("cube", vec3f_255, ShadingType::PIXEL, vec3f_0, vec3f_0, false, 0, 0, true, vec3f_0, vec3f_0, { 1, 1, 1 });
	graphics::addSpotLight(vec3f_255, { 0.8, 1, -2 }, { -0.25, -0.25, 1 });
	//*/

	/*
	graphics::loadModel("cube", "examples/assets/models/cube.obj");
	graphics::loadTexture("grass", "examples/assets/textures/grass.jpg");
	graphics::setAmbientLightColor({ 25, 25, 25 });
	graphics::addPointLight(vec3f_255, vec3f_0, 0.1f);
	graphics::addEntity("cube", "grass", ShadingType::FLAT,    { -3, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0, 0.1, 0 });
	graphics::addEntity("cube", "grass", ShadingType::VERTEX,  { -1, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0.1, 0.1, 0 });
	graphics::addEntity("cube", "grass", ShadingType::PIXEL,   {  1, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0, 0.1, 0.1 });
	graphics::addEntity("cube", "grass", ShadingType::PIXEL_S, {  3, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0.1, 0.1, 0.1 });
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
		graphics::renderFrame(dtime);
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
