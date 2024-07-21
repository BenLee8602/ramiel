#include <GLFW/glfw3.h>

#include <ramiel/ramiel.h>
using namespace ramiel;

constexpr size_t width = 1280;
constexpr size_t height = 720;

constexpr float cameraSpeedNormal = 2.0f;
constexpr float cameraSpeedFaster = 10.0f;
constexpr float cameraRotationSpeed = 1.57079f;


void cameraControls(GLFWwindow* window, float dtime) {
    // reset pos and rot
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        setPos(Vec3f());
        setRot(Vec3f());
        return;
    }

    // move faster
    float cameraSpeed = (
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ?
        cameraSpeedFaster : cameraSpeedNormal
    );
    float dPos = cameraSpeed * dtime;
    float dRot = cameraRotationSpeed * dtime;

    Vec3f pos = getPos();
    Vec3f rot = getRot();

    // move left
    if (glfwGetKey(window, GLFW_KEY_A)) {
        pos[X] -= dPos * std::cos(rot[Y]);
        pos[Z] -= dPos * std::sin(rot[Y]);
    }

    // move right
    if (glfwGetKey(window, GLFW_KEY_D)) {
        pos[X] += dPos * std::cos(rot[Y]);
        pos[Z] += dPos * std::sin(rot[Y]);
    }

    // move down
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
        pos[Y] -= dPos;
    }

    // move up
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        pos[Y] += dPos;
    }

    // move backward
    if (glfwGetKey(window, GLFW_KEY_S)) {
        pos[X] += dPos * std::sin( rot[Y]);
        pos[Y] += dPos * std::sin(-rot[X]);
        pos[Z] -= dPos * std::cos( rot[Y]);
    }

    // move forward
    if (glfwGetKey(window, GLFW_KEY_W)) {
        pos[X] -= dPos * std::sin( rot[Y]);
        pos[Y] -= dPos * std::sin(-rot[X]);
        pos[Z] += dPos * std::cos( rot[Y]);
    }

    // turn down
    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        rot[X] = std::max(rot[X] - dRot, -1.57079f);
    }

    // turn up
    if (glfwGetKey(window, GLFW_KEY_UP)) {
        rot[X] = std::min(rot[X] + dRot, 1.57079f);
    }

    // turn right
    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        rot[Y] -= dRot;
    }

    // turn left
    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        rot[Y] += dRot;
    }

    setPos(pos);
    setRot(rot);
}


void getFrameRGB(uint8_t* frame) {
    auto color = getColorBuffer();
    for (size_t i = 0; i < getBufferSize(); ++i) {
        *frame++ = color[i][R];
        *frame++ = color[i][G];
        *frame++ = color[i][B];
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

    setRes({ width, height });

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

        cameraControls(window, (float)dtime);
        simulatePhysics(dtime);
        renderFrame();
        getFrameRGB(frame);
        
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
