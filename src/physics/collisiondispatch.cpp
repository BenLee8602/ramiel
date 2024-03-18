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
        CollisionHandlerFactory,
        CollisionHandlerTableHash
    > collisionHandlers = {
        { { typeid(SphereCollider), typeid(SphereCollider) }, collide<Collide_Sph_Sph> },
        { { typeid(AabbCollider),   typeid(AabbCollider)   }, collide<Collide_Aabb_Aabb> }
    };

}


namespace ramiel {

    CollisionHandler_P getCollisionHandler(Collider* c1, Collider* c2) {
        CollisionHandlerFactory factory = getCollisionHandler(c1->getType(), c2->getType());
        if (!factory) return nullptr;
        return factory(c1, c2);
    }

    
    CollisionHandlerFactory getCollisionHandler(ColliderType type1, ColliderType type2) {
        auto handler = collisionHandlers.find({ type1, type2 });
        if (handler != collisionHandlers.end()) return handler->second;

        handler = collisionHandlers.find({ type2, type1 });
        if (handler != collisionHandlers.end()) return handler->second;

        return nullptr;
    }


    void setCollisionHandler(ColliderType type1, ColliderType type2, CollisionHandlerFactory handler) {
        if (collisionHandlers.find({ type2, type1 }) != collisionHandlers.end())
            collisionHandlers[{ type2, type1 }] = handler;
        else collisionHandlers[{ type1, type2 }] = handler;
    }


    void deleteCollisionHandler(ColliderType type1, ColliderType type2) {
        collisionHandlers.erase({ type1, type2 });
        collisionHandlers.erase({ type2, type1 });
    }

}
