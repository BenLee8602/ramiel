#pragma once

#include <cstdint>

namespace ramiel {

    class Constraint;
    class PhysicsEntity;

    void addConstraint(Constraint* constraint);
    void removeConstraint(Constraint* constraint);

    void addEntity(PhysicsEntity* entity);
    void removeEntity(PhysicsEntity* entity);

    void simulatePhysics(float dt, uint32_t steps);

}