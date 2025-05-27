#include <ramiel/physics.h>
#include "physics.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    bool running = false;

}

namespace ramiel {

    void simStart() {
        running = true;
    }


    void simStop() {
        running = false;
    }


    void simStep(float dtime) {
        if (!running) return;
        simulatePhysics(dtime);
        renderNeeded();
    }

}
