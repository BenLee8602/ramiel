#include <chrono>
#include <thread>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

#include "command.h"
#include "window.h"
#include "graphics.h"
#include "physics.h"
#include "task.h"
using namespace ramiel;


void cameraControls(float dtime) {
    constexpr float cameraSpeedNormal = 2.0f;
    constexpr float cameraSpeedFaster = 10.0f;
    constexpr float cameraRotationSpeed = 1.57079f;

    // reset pos and rot
    if (keyUp(Key::Q)) {
        setPos(Vec3f());
        setRot(Vec3f());
        renderNeeded();
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
        renderNeeded();
    }

    // move right
    if (keyPressed(Key::D)) {
        pos[X] += dPos * std::cos(rot[Y]);
        pos[Z] += dPos * std::sin(rot[Y]);
        renderNeeded();
    }

    // move down
    if (keyPressed(Key::CONTROL)) {
        pos[Y] -= dPos;
        renderNeeded();
    }

    // move up
    if (keyPressed(Key::SPACE)) {
        pos[Y] += dPos;
        renderNeeded();
    }

    // move backward
    if (keyPressed(Key::S)) {
        pos[X] += dPos * std::sin( rot[Y]);
        pos[Y] += dPos * std::sin(-rot[X]);
        pos[Z] -= dPos * std::cos( rot[Y]);
        renderNeeded();
    }

    // move forward
    if (keyPressed(Key::W)) {
        pos[X] -= dPos * std::sin( rot[Y]);
        pos[Y] -= dPos * std::sin(-rot[X]);
        pos[Z] += dPos * std::cos( rot[Y]);
        renderNeeded();
    }

    // turn down
    if (keyPressed(Key::DOWN)) {
        rot[X] = std::max(rot[X] - dRot, -1.57079f);
        renderNeeded();
    }

    // turn up
    if (keyPressed(Key::UP)) {
        rot[X] = std::min(rot[X] + dRot, 1.57079f);
        renderNeeded();
    }

    // turn right
    if (keyPressed(Key::RIGHT)) {
        rot[Y] -= dRot;
        renderNeeded();
    }

    // turn left
    if (keyPressed(Key::LEFT)) {
        rot[Y] += dRot;
        renderNeeded();
    }

    setPos(pos);
    setRot(rot);
}


void runCommandLine() {
    std::string command;
    while (true) {
        std::cout << getPath() << "> ";
        std::getline(std::cin, command);
        runCommand(command);
    }
}


int main() {
    initWindow();
    std::thread(runCommandLine).detach();

    auto frameTimeStart = std::chrono::steady_clock::now();
    auto frameTimeEnd = std::chrono::steady_clock::now();

    while (windowGood()) {
        execTasks();

        frameTimeEnd = std::chrono::steady_clock::now();
        double dtime = (frameTimeEnd - frameTimeStart).count() / 1e9;
        frameTimeStart = frameTimeEnd;

        cameraControls((float)dtime);

        simStep(dtime);
        render();

        updateFrame();
        pollWindowMessages();
    }

    return 0;
}
