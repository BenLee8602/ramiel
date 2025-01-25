#pragma once

#include <cstdint>
#include "collision.h"
#include "broadcollision.h"

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

    void addCollider(Collider* collider);
    void removeCollider(Collider* collider);

    void addCollisionDetector(
        Collider::Type t1,
        Collider::Type t2,
        CollisionDetector detector
    );
    void removeCollisionDetector(
        Collider::Type t1,
        Collider::Type t2
    );
    template<class C1, class C2>
    void addCollisionDetector(CollisionDetector detector) {
        addCollisionDetector(typeid(C1), typeid(C2), detector);
    }
    template<class C1, class C2>
    void removeCollisionDetector() {
        removeCollisionDetector(typeid(C1), typeid(C2));
    }

    void setBroadCollisionFn(BroadCollisionFn fn);

    void simulatePhysics(float dt);
    void simulatePhysics(float dt, uint32_t steps);

}