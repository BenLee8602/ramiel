#include "ramiel.h"
#include "graphics/vertexshader.h"
#include "graphics/pixelshader.h"
using namespace ramiel;

namespace {

    std::vector<Kinematics*> dynamicObjects;
    std::vector<Collider*> colliders;

}

namespace ramiel {

    bool addDynamicObject(Kinematics* dynamicObject) {
        if (!dynamicObject) return false;
        dynamicObjects.emplace_back(dynamicObject);
        return true;
    }


    bool removeDynamicObject(Kinematics* dynamicObject) {
        auto i = std::find(dynamicObjects.begin(), dynamicObjects.end(), dynamicObject);
        if (i == dynamicObjects.end()) return false;
        dynamicObjects.erase(i);
        return true;
    }


    bool addCollider(Collider* collider) {
        if (!collider) return false;
        colliders.emplace_back(collider);
        return true;
    }


    bool removeCollider(Collider* collider) {
        auto i = std::find(colliders.begin(), colliders.end(), collider);
        if (i == colliders.end()) return false;
        colliders.erase(i);
        return true;
    }

    
    void simulatePhysics(float dtime) {
        for (auto& o : dynamicObjects) o->step(dtime);

        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                CollisionHandler_P handler = getCollisionHandler(colliders[i], colliders[j]);
                if (handler && handler->detect()) handler->resolve();
            }
        }
    }

}
