#include <iostream>
#include <GLFW/glfw3.h>
#include <graphicsbl.h>
using namespace bl;

constexpr size_t width = 1280;
constexpr size_t height = 720;


void initScene() {
	//*
	GraphicsBL::loadModel("terrain", "examples/assets/models/terrain.obj", { -64, 0, -64 });
	GraphicsBL::setAmbientLightColor({ 100, 80, 100 });
	GraphicsBL::setBackgroundColor({ 150, 110, 110 });
	GraphicsBL::addDirLight({ 155, 40, 0 }, { -10, 1, 0 });
	GraphicsBL::addEntity("terrain", vec3f_255, ShadingType::PIXEL);
	GraphicsBL::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
	//*/

	/*
	GraphicsBL::loadModel("cube", "examples/assets/models/cube.obj");
	GraphicsBL::loadTexture("grass", "examples/assets/textures/grass.jpg");
	GraphicsBL::setAmbientLightColor({ 25, 25, 25 });
	GraphicsBL::addPointLight(vec3f_255, vec3f_0, 0.1f);
	GraphicsBL::addEntity("cube", "grass", ShadingType::FLAT,    { -3, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0, 0.1, 0 });
	GraphicsBL::addEntity("cube", "grass", ShadingType::VERTEX,  { -1, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0.1, 0.1, 0 });
	GraphicsBL::addEntity("cube", "grass", ShadingType::PIXEL,   {  1, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0, 0.1, 0.1 });
	GraphicsBL::addEntity("cube", "grass", ShadingType::PIXEL_S, {  3, 0, 4 }, vec3f_0, false, 0.5f, 1.0f, true, vec3f_0, vec3f_0, vec3f_0, { 0.1, 0.1, 0.1 });
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

	GLFWwindow* window = glfwCreateWindow(width, height, "GraphicsBL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GraphicsBL::setBufferSize({ width, height });
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
		GraphicsBL::setControls(controls);
		GraphicsBL::renderFrame(dtime);
		GraphicsBL::getFrameRGB(frame);

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
