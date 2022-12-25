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

	/*
	Scene scene;
    scene.camera.res({ width, height });

    scene.loadMesh<Vertex_Mesh_TN>("examples/assets/models/cube.obj", "cube", true, true);
	scene.loadTexture("examples/assets/textures/brickwall_texture.jpg", "brickTexture");
	scene.loadNormalMap("examples/assets/textures/brickwall_normal.jpg", "brickNormal");
	
	scene.ambientLight = { 25.0f, 25.0f, 25.0f };
	scene.addLight(new Light_Pt(vec3f_255, 4.0f, { -6, 2, 2 }, 0.2f));
	scene.addLight(new Light_Pt(vec3f_255, 4.0f, {  0, 2, 2 }, 0.2f));
	scene.addLight(new Light_Pt(vec3f_255, 4.0f, {  6, 2, 2 }, 0.2f));

    scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerTri(
			scene.camera,
			new PhysicsObject(scene, { -9, 0, 4 })
		),
        PS_PerTri(
			scene.getLightingList(1, 0.0f),
			{ 100, 90, 70 }
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerTri_Textured(
			scene.camera,
			new PhysicsObject(scene, { -7, 0, 4 })
		),
        PS_PerTri_Textured(
			scene.getLightingList(1, 0.0f),
			scene.getTexture("brickTexture")
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerVertex(
			scene.camera,
			scene.getLightingList(1, 0.0f),
			{ 100, 90, 70 },
			new PhysicsObject(scene, { -5, 0, 4 })
		),
        PS_PerVertex()
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerVertex_Textured(
			scene.camera,
			scene.getLightingList(1, 0.0f),
			new PhysicsObject(scene, { -3, 0, 4 })
		),
        PS_PerVertex_Textured(scene.getTexture("brickTexture"))
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel(
			scene.camera,
			new PhysicsObject(scene, { -1, 0, 4 })
		),
        PS_PerPixel(
			scene.getLightingList(1, 0.0f),
			{ 100, 90, 70 }
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(
			scene.camera,
			new PhysicsObject(scene, { 1, 0, 4 })
		),
        PS_PerPixel_Textured(
			scene.getLightingList(1, 0.0f),
			scene.getTexture("brickTexture")
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(
			scene.camera,
			new PhysicsObject(scene, { 3, 0, 4 })
		),
        PS_PerPixel_NormalMapped(
			scene.getLightingList(1, 0.0f),
			scene.getTexture("brickNormal"),
			{ 100, 90, 70 }
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Textured(
			scene.camera,
			new PhysicsObject(scene, { 5, 0, 4 })
		),
        PS_PerPixel_Textured_NormalMapped(
			scene.getLightingList(1, 0.0f),
			scene.getTexture("brickTexture"),
			scene.getTexture("brickNormal")
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Smooth(
			scene.camera,
			new PhysicsObject(scene, { 7, 0, 4 })
		),
        PS_PerPixel_Smooth(
			scene.getLightingList(1, 0.0f),
			{ 100, 90, 70 }
		)
    );
	
	scene.addEntity<Vertex_Mesh_TN>(
        "cube",
        VS_PerPixel_Smooth_Textured(
			scene.camera,
			new PhysicsObject(scene, { 9, 0, 4 })
		),
        PS_PerPixel_Smooth_Textured(
			scene.getLightingList(1, 0.0f),
			scene.getTexture("brickTexture")
		)
    );
	//*/

	//*
	Scene scene;
	scene.camera.res({ width, height });

	scene.loadMesh<Vertex_Mesh>("examples/assets/models/sphere.obj", "sphere");

	scene.ambientLight = { 25, 25, 25 };
	scene.addLight(new Light_Dir(vec3f_255, 1.0f, { 0, 0, 1 }));

	scene.addEntity<Vertex_Mesh>(
		"sphere",
		VS_PerTri(
			scene.camera,
			new SphereCollider(1.0f, scene, { 0, 0, 32 }, vec3f_0, false)
		),
		PS_PerTri(
			scene.getLightingList(1, 0.0f),
			{ 255, 25, 25 }
		)
	);

	SphereCollider* sc1 = new SphereCollider(
		1.0f,
		scene,
		{ 0, 4, 32 },
		vec3f_0,
		true
	);
	sc1->posAcc = { 0, -9.8, 0 };
	scene.addEntity<Vertex_Mesh>(
		"sphere",
		VS_PerTri(
			scene.camera,
			sc1
		),
		PS_PerTri(
			scene.getLightingList(1, 0.0f),
			{ 25, 255, 25 }
		)
	);

	SphereCollider* sc2 = new SphereCollider(
		1.0f,
		scene,
		{ 0.5, 16, 32 },
		vec3f_0,
		true
	);
	sc2->posAcc = { 0, -9.8, 0 };
	scene.addEntity<Vertex_Mesh>(
		"sphere",
		VS_PerTri(
			scene.camera,
			sc2
		),
		PS_PerTri(
			scene.getLightingList(1, 0.0f),
			{ 25, 25, 255 }
		)
	);

	//*/

	uint8_t* frame = new uint8_t[width * height * 3];

	glfwSetTime(0);
	double frameStart = glfwGetTime();
	double frameEnd = glfwGetTime();
	double dtime = frameEnd - frameStart;
	
	while (!glfwWindowShouldClose(window)) {
		frameStart = glfwGetTime();

		cameraControls(window, (float)dtime, scene.camera);
		scene.simulatePhysics(dtime);
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
