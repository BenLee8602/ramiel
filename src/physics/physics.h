#pragma once

#include <cstdint>

namespace ramiel {

    class Constraint;
    class Particle;
    class RigidBody;

    void addConstraint(Constraint* constraint);
    void removeConstraint(Constraint* constraint);

    void addEntity(Particle* entity);
    void removeEntity(Particle* entity);

    void addEntity(RigidBody* entity);
    void removeEntity(RigidBody* entity);

    void simulatePhysics(float dt, uint32_t steps);

}