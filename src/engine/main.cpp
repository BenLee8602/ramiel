#include <chrono>
#include <thread>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

#include "engine.h"
#include "command.h"
#include "window.h"
using namespace ramiel;


void cameraControls(float dtime) {
    constexpr float cameraSpeedNormal = 2.0f;
    constexpr float cameraSpeedFaster = 10.0f;
    constexpr float cameraRotationSpeed = 1.57079f;

    // reset pos and rot
    if (keyPressed(Key::Q)) {
        setPos(Vec3f());
        setRot(Vec3f());
        return;
    }

    // move faster
    float cameraSpeed = (
        keyPressed(Key::SHIFT) ?
        cameraSpeedFaster : cameraSpeedNormal
    );
    float dPos = cameraSpeed * dtime;
    float dRot = cameraRotationSpeed * dtime;

    Vec3f pos = getPos();
    Vec3f rot = getRot();

    // move left
    if (keyPressed(Key::A)) {
        pos[X] -= dPos * std::cos(rot[Y]);
        pos[Z] -= dPos * std::sin(rot[Y]);
    }

    // move right
    if (keyPressed(Key::D)) {
        pos[X] += dPos * std::cos(rot[Y]);
        pos[Z] += dPos * std::sin(rot[Y]);
    }

    // move down
    if (keyPressed(Key::CONTROL)) {
        pos[Y] -= dPos;
    }

    // move up
    if (keyPressed(Key::SPACE)) {
        pos[Y] += dPos;
    }

    // move backward
    if (keyPressed(Key::S)) {
        pos[X] += dPos * std::sin( rot[Y]);
        pos[Y] += dPos * std::sin(-rot[X]);
        pos[Z] -= dPos * std::cos( rot[Y]);
    }

    // move forward
    if (keyPressed(Key::W)) {
        pos[X] -= dPos * std::sin( rot[Y]);
        pos[Y] -= dPos * std::sin(-rot[X]);
        pos[Z] += dPos * std::cos( rot[Y]);
    }

    // turn down
    if (keyPressed(Key::DOWN)) {
        rot[X] = std::max(rot[X] - dRot, -1.57079f);
    }

    // turn up
    if (keyPressed(Key::UP)) {
        rot[X] = std::min(rot[X] + dRot, 1.57079f);
    }

    // turn right
    if (keyPressed(Key::RIGHT)) {
        rot[Y] -= dRot;
    }

    // turn left
    if (keyPressed(Key::LEFT)) {
        rot[Y] += dRot;
    }

    setPos(pos);
    setRot(rot);
}


void getFrameRGB(uint8_t* frame) {
    auto color = getColorBuffer();
    for (size_t i = 0; i < getBufferSize(); ++i) {
        *frame++ = std::min(color[i][B], 255.0f);
        *frame++ = std::min(color[i][G], 255.0f);
        *frame++ = std::min(color[i][R], 255.0f);
    }
}


void runCommandLine() {
    std::string command;
    while (true) {
        std::cout << get_path() << "> ";
        std::getline(std::cin, command);
        runCommand(command);
    }
}


int main() {
    if (!initWindow()) return 0;
    std::thread(runCommandLine).detach();

    setRes({
        static_cast<unsigned>(windowWidth()),
        static_cast<unsigned>(windowHeight())
    });

    const char* meshFile = "example/assets/models/cube.obj";
    const char* textureFile = "example/assets/textures/brickwall_texture.jpg";

    auto mesh = std::make_shared<Mesh>(meshFile);
    auto texture = std::make_shared<Texture>(textureFile, rgb1);

    Entity entity(
        mesh,
        std::make_unique<VertexShaderTextured>(translate(Vec3f{ 0, 0, 4 })),
        std::make_unique<PixelShaderTextured>(texture, 8.0f, 1.0f, Vec3f{})
    );

    setAmbientLight({ 25, 10, 20 });
    addLight(new PointLight(Vec3f{ 255, 100, 200 }, 4.0f, Vec3f{ 1, 1.5, 2 }, 0.5f));

    std::vector<uint8_t> frame(windowWidth() * windowHeight() * 3);
    auto frameTimeStart = std::chrono::steady_clock::now();
    auto frameTimeEnd = std::chrono::steady_clock::now();

    while (windowGood()) {
        pollWindowMessages();

        frameTimeEnd = std::chrono::steady_clock::now();
        double dtime = (frameTimeEnd - frameTimeStart).count() / 1e9;
        frameTimeStart = frameTimeEnd;

        cameraControls((float)dtime);

        simulatePhysics(dtime);

        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), Vec3f{});
        std::fill(getDepthBuffer(), getDepthBuffer() + getBufferSize(), getZ1());
        entity.draw();
        getFrameRGB(frame.data());

        drawToWindow(frame.data());
    }

    return 0;
}
