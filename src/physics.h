#pragma once

#include <vector>
#include "physicsobj.h"

namespace ramiel::physics {

    extern std::vector<PhysicsObj*> physicsObjs;
    extern std::vector<PhysicsObj*> dynamicObjs;
    extern std::vector<PhysicsObj*> collidableObjs;

    void simulatePhysics();
    void simulateDynamics();
    void simulateCollisions();

}
