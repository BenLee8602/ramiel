#include <unordered_map>
#include "collisiondispatch.h"
using namespace ramiel;

namespace {

    struct CollisionHandlerTableHash {
        size_t operator()(const std::pair<ColliderType, ColliderType>& key) const {
            size_t hash1 = key.first.hash_code();
            size_t hash2 = key.second.hash_code();
            return hash1 ^ (hash2 << 1);
        }
    };

    std::unordered_map<
        std::pair<ColliderType, ColliderType>,
        CollisionHandler,
        CollisionHandlerTableHash
    > collisionHandlers = {
        { { typeid(SphereCollider), typeid(SphereCollider) }, collideSphereSphere },
        { { typeid(AabbCollider),   typeid(AabbCollider)   }, collideAabbAabb     }
    };

}


namespace ramiel {

    void dispatchCollision(Collider* collider1, Collider* collider2) {
        ColliderType type1 = collider1->getType();
        ColliderType type2 = collider2->getType();
        
        auto handler = collisionHandlers.find({ type1, type2 });
        if (handler != collisionHandlers.end()) {
            handler->second(collider1, collider2);
            return;
        }

        handler = collisionHandlers.find({ type1, type2 });
        if (handler != collisionHandlers.end()) handler->second(collider2, collider1);
    }

    
    CollisionHandler getCollisionHandler(ColliderType type1, ColliderType type2) {
        auto handler = collisionHandlers.find({ type1, type2 });
        if (handler != collisionHandlers.end()) return handler->second;

        handler = collisionHandlers.find({ type2, type1 });
        if (handler != collisionHandlers.end()) return handler->second;

        return CollisionHandler();
    }


    void setCollisionHandler(ColliderType type1, ColliderType type2, CollisionHandler handler) {
        if (collisionHandlers.find({ type2, type1 }) != collisionHandlers.end())
            collisionHandlers[{ type2, type1 }] = handler;
        else collisionHandlers[{ type1, type2 }] = handler;
    }


    void deleteCollisionHandler(ColliderType type1, ColliderType type2) {
        collisionHandlers.erase({ type1, type2 });
        collisionHandlers.erase({ type2, type1 });
    }

}
