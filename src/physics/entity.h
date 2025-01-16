#pragma once

#include <ramiel/math.h>

namespace ramiel {

    class Particle {
    public:
        Vec3f pos;
        Vec3f vel;
        float wass;

        Vec3f posPrev;

        void integrate(float dt);
        void update(float dt);
    };

}
