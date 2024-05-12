#include <GLFW/glfw3.h>

#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;

constexpr float cameraSpeedNormal = 2.0f;
constexpr float cameraSpeedFaster = 10.0f;
constexpr float cameraRotationSpeed = 1.57079f;


void cameraControls(GLFWwindow* window, float dtime, Camera& camera) {
	// move faster
	float cameraSpeed = (
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ?
		cameraSpeedFaster : cameraSpeedNormal
	);
	float pos = cameraSpeed * dtime;
	float rot = cameraRotationSpeed * dtime;

	// reset pos and rot
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera.pos = Vec3f();
		camera.rot = Vec3f();
	}

	// move left
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera.pos[X] -= pos * std::cos(camera.rot[Y]);
		camera.pos[Z] -= pos * std::sin(camera.rot[Y]);
	}

	// move right
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera.pos[X] += pos * std::cos(camera.rot[Y]);
		camera.pos[Z] += pos * std::sin(camera.rot[Y]);
	}

	// move down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		camera.pos[Y] -= pos;
	}

	// move up
	if (glfwGetKey(window, GLFW_KEY_SPACE)) {
		camera.pos[Y] += pos;
	}

	// move backward
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera.pos[X] += pos * std::sin( camera.rot[Y]);
		camera.pos[Y] += pos * std::sin(-camera.rot[X]);
		camera.pos[Z] -= pos * std::cos( camera.rot[Y]);
	}

	// move forward
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera.pos[X] -= pos * std::sin( camera.rot[Y]);
		camera.pos[Y] -= pos * std::sin(-camera.rot[X]);
		camera.pos[Z] += pos * std::cos( camera.rot[Y]);
	}

	// turn down
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		camera.rot[X] = std::max(camera.rot[X] - rot, -1.57079f);
	}

	// turn up
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		camera.rot[X] = std::min(camera.rot[X] + rot, 1.57079f);
	}

	// turn right
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		camera.rot[Y] -= rot;
	}

	// turn left
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		camera.rot[Y] += rot;
	}
}


int main() {
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(width, height, "ramiel", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	cam().setRes({ width, height });

	loadMesh<MeshVertex>("examples/assets/models/terrain.obj", "terrain");
	loadMesh<MeshVertexT>("examples/assets/models/cube.obj", "cube");

	loadTexture("examples/assets/textures/brickwall_texture.jpg", "brickwall", rgb1);

	setAmbientLight({ 100, 80, 100 });
	addLight(new DirectionalLight({ 155, 40, 0 }, 1.0f, { -10, 1, 0 }));
	addEntity<MeshVertex>(
		"terrain",
		VS_PerTri(new PhysicsObject({ -64, 0, -64 })),
		PS_PerTri(getLightingList(8, 1.0f), Vec3f{ 255, 255, 255 })
	);
	addEntity<MeshVertexT>(
		"cube",
		VS_PerPixel_Textured(new PhysicsObject()),
		PS_PerPixel_Textured(getLightingList(8, 1.0f), getTexture("brickwall"))
	);
	addEffect(new Fog({ 150, 110, 110 }, 20, 100));

	uint8_t* frame = new uint8_t[width * height * 3];

	glfwSetTime(0);
	double frameStart = glfwGetTime();
	double frameEnd = glfwGetTime();
	double dtime = frameEnd - frameStart;
	
	while (!glfwWindowShouldClose(window)) {
		frameStart = glfwGetTime();

		cameraControls(window, (float)dtime, cam());
		simulatePhysics(dtime);
		renderFrame();
		cam().getFrameRGB(frame);
		
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, frame);
		glfwSwapBuffers(window);
		glfwPollEvents();

		frameEnd = glfwGetTime();
		dtime = frameEnd - frameStart;
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	delete[] frame;

	destroy();
	return 0;
}
