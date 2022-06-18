#include <ramiel/ramiel.h>
#include "physics.h"

namespace ramiel {

    float physics::dtime = 0.0f;
    std::vector<PhysicsObj*> physics::physicsObjs;
    std::vector<PhysicsObj*> physics::dynamicObjs;
    std::vector<Collider*>   physics::collidableObjs;


    void physics::simulateDynamics() {
        for (auto d : dynamicObjs) {
            d->step();
        }
    }


    void physics::simulateCollisions() {
        for (size_t i = 0; i < collidableObjs.size(); ++i) {
            for (size_t j = i + 1; j < collidableObjs.size(); ++j) {
                collidableObjs[i]->collideWith(collidableObjs[j]);
            }
        }
    }


    void physics::simulatePhysics(float dtime) {
        physics::dtime = dtime;
        simulateDynamics();
        simulateCollisions();
    }

}
