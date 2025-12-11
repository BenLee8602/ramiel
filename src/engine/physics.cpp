#include <chrono>

#include <ramiel/physics.h>
#include "physics.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    auto frameTimeStart = std::chrono::steady_clock::now();
    auto frameTimeEnd = std::chrono::steady_clock::now();
    float dtime = 0.0f;

    bool running = false;

}

namespace ramiel {

    float getDtime() {
        return dtime;
    }

    void setDtime() {
        frameTimeEnd = std::chrono::steady_clock::now();
        dtime = (frameTimeEnd - frameTimeStart).count() / 1e9f;
        frameTimeStart = frameTimeEnd;
    }


    void simStart() {
        running = true;
    }

    void simStop() {
        running = false;
    }


    void simStep() {
        if (!running) return;
        simulatePhysics(dtime);
        renderNeeded();
    }

}
