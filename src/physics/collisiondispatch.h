#pragma once

#include <memory>
#include "collider.h"
#include "collisionhandle.h"

namespace ramiel {

    using CollisionHandler_P = std::unique_ptr<CollisionHandler>;
    using CollisionHandlerFactory = std::function<CollisionHandler_P(Collider*, Collider*)>;

    template<class CollisionHandlerType>
    CollisionHandler_P collide(Collider* c1, Collider* c2) {
        return std::make_unique<CollisionHandlerType>(c1, c2);
    }
    
    CollisionHandler_P getCollisionHandler(Collider* c1, Collider* c2);
    CollisionHandlerFactory getCollisionHandler(ColliderType type1, ColliderType type2);
    void setCollisionHandler(ColliderType type1, ColliderType type2, CollisionHandlerFactory handler);
    void deleteCollisionHandler(ColliderType type1, ColliderType type2);

}
