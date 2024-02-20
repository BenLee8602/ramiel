#pragma once

#include <functional>
#include "collider.h"

namespace ramiel {

    typedef std::function<void(Collider*, Collider*)> CollisionHandler;

    void collideSphereSphere(Collider* collider1, Collider* collider2);
    void collideAabbAabb(Collider* collider1, Collider* collider2);

}
