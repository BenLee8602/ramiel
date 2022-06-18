#pragma once

#include <vector>
#include "physicsobj.h"

namespace ramiel::physics {

    extern float dtime;
    extern std::vector<PhysicsObj*> physicsObjs;
    extern std::vector<PhysicsObj*> dynamicObjs;
    extern std::vector<Collider*>   collidableObjs;

    void simulateDynamics();
    void simulateCollisions();

}
