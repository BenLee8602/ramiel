#pragma once

#include <cstdint>

namespace ramiel {

    class Constraint;
    class Particle;

    void addConstraint(Constraint* constraint);
    void removeConstraint(Constraint* constraint);

    void addEntity(Particle* entity);
    void removeEntity(Particle* entity);

    void simulatePhysics(float dt, uint32_t steps);

}