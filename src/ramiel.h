#pragma once

#include <unordered_map>
#include <fstream>

#include <ramiel/math.h>
#include <ramiel/file.h>
#include <ramiel/data.h>
#include <ramiel/graphics.h>
#include <ramiel/physics.h>

namespace ramiel {

    bool addDynamicObject(Kinematics* dynamicObject);
    bool removeDynamicObject(Kinematics* dynamicObject);

    bool addCollider(Collider* collider);
    bool removeCollider(Collider* collider);

    void simulatePhysics(float dtime);

}
