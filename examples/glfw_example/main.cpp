#include <GLFW/glfw3.h>

#include <ramiel/core.h>
#include <ramiel/shaders.h>
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
		camera.pos = vec3f_0;
		camera.rot = vec3f_0;
	}

	// move left
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera.pos[X] -= pos * camera.rot.cos()[Y];
		camera.pos[Z] -= pos * camera.rot.sin()[Y];
	}

	// move right
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera.pos[X] += pos * camera.rot.cos()[Y];
		camera.pos[Z] += pos * camera.rot.sin()[Y];
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
		camera.pos[X] += pos *  camera.rot.sin()[Y];
		camera.pos[Y] += pos * -camera.rot.sin()[X];
		camera.pos[Z] -= pos *  camera.rot.cos()[Y];
	}

	// move forward
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera.pos[X] -= pos *  camera.rot.sin()[Y];
		camera.pos[Y] -= pos * -camera.rot.sin()[X];
		camera.pos[Z] += pos *  camera.rot.cos()[Y];
	}

	// turn down
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		if (camera.rot[X] <  1.57079f) camera.rot += { -rot, 0, 0 };
	}

	// turn up
	if (glfwGetKey(window, GLFW_KEY_UP)) {
		if (camera.rot[X] > -1.57079f) camera.rot += {  rot, 0, 0 };
	}

	// turn right
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		camera.rot += { 0, -rot, 0 };
	}

	// turn left
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		camera.rot += { 0,  rot, 0 };
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

	Scene scene;
    scene.loadMesh<Vertex_Mesh_TN>("examples/assets/models/cube.obj", "cube", true, true);
	scene.loadTexture("examples/assets/textures/brickwall_texture.jpg", "brickTexture");
	scene.loadNormalMap("examples/assets/textures/brickwall_normal.jpg", "brickNormal");

	/*
    scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerTri(scene.camera),
        PS_PerTri(scene.getLightingList(1, 0.0f), vec3f_255)
    );
	//*/

	/*
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerTri_Textured(scene.camera),
        PS_PerTri_Textured(scene.getLightingList(1, 0.0f), scene.getTexture("brickTexture"))
    );
	//*/

	/*
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerVertex(scene.camera, scene.getLightingList(1, 0.0f)),
        PS_PerVertex()
    );
	//*/

	//* texture distorted 1
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerVertex_Textured(scene.camera, scene.getLightingList(1, 0.0f)),
        PS_PerVertex_Textured(scene.getTexture("brickTexture"))
    );
	//*/

	/*
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel(scene.camera),
        PS_PerPixel(scene.getLightingList(1, 0.0f), vec3f_255)
    );
	//*/

	/* texture distorted 2
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(scene.camera),
        PS_PerPixel_Textured(scene.getLightingList(1, 0.0f), scene.getTexture("brickTexture"))
    );
	//*/

	/* texture distorted 2
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(scene.camera),
        PS_PerPixel_NormalMapped(scene.getLightingList(1, 0.0f), scene.getTexture("brickNormal"), vec3f_255)
    );
	//*/

	/* texture distorted 2
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(scene.camera),
        PS_PerPixel_Textured_NormalMapped(scene.getLightingList(1, 0.0f), scene.getTexture("brickTexture"), scene.getTexture("brickNormal"))
    );
	//*/

	/*
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Smooth(scene.camera),
        PS_PerPixel_Smooth(scene.getLightingList(1, 0.0f), vec3f_255)
    );
	//*/

	/*
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Smooth_Textured(scene.camera),
        PS_PerPixel_Smooth_Textured(scene.getLightingList(1, 0.0f), scene.getTexture("brickTexture"))
    );
	//*/
	
    scene.camera.res({ width, height });
	scene.camera.pos = { 1.0f, 1.0f, -4.0f };
	scene.ambientLight = { 50.0f, 50.0f, 50.0f };
	scene.addLight(new Light_Pt(vec3f_255, 1.0f, { 1.0f, 1.0f, -4.0f }, 0.1f));

	uint8_t* frame = new uint8_t[width * height * 3];

	glfwSetTime(0);
	double frameStart = glfwGetTime();
	double frameEnd = glfwGetTime();
	double dtime = frameEnd - frameStart;
	
	while (!glfwWindowShouldClose(window)) {
		frameStart = glfwGetTime();

		cameraControls(window, (float)dtime, scene.camera);
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
