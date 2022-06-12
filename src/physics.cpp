#include "physics.h"

namespace ramiel {

    std::vector<PhysicsObj*> physics::physicsObjs;
    std::vector<PhysicsObj*> physics::dynamicObjs;
    std::vector<PhysicsObj*> physics::collidableObjs;


    void physics::simulatePhysics() {
        simulateDynamics();
        simulateCollisions();
    }


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

}
