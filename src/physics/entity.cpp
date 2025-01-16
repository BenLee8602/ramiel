#include "entity.h"

namespace ramiel {

    void Particle::integrate(float dt) {
        vel[Y] -= 9.8f * dt;
        posPrev = pos;
        pos += vel * dt;
    }


    void Particle::update(float dt) {
        vel = (pos - posPrev) / dt;
    }

}
