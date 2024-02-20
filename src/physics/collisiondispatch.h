#pragma once

#include "collider.h"
#include "collision.h"

namespace ramiel {
    
    void dispatchCollision(Collider* collider1, Collider* collider2);
    
    CollisionHandler getCollisionHandler(ColliderType type1, ColliderType type2);
    void setCollisionHandler(ColliderType type1, ColliderType type2, CollisionHandler handler);
    void deleteCollisionHandler(ColliderType type1, ColliderType type2);

}
