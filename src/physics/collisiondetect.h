#pragma once

#include <functional>
#include <ramiel/math.h>

namespace ramiel {

    class Constraint;
    class Collider;

    using CollisionDetector = std::function<
        Constraint*(Collider*, Collider*)
    >;


    CollisionDetector collideParticlePlane();
    CollisionDetector collideParticleSphere();
    CollisionDetector collideParticleBox();
    CollisionDetector collidePlaneSphere();
    CollisionDetector collidePlaneBox();
    CollisionDetector collideSphereSphere();
    CollisionDetector collideSphereBox();
    CollisionDetector collideBoxBox();

}
