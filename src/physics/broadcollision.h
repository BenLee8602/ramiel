#pragma once

#include <functional>
#include <vector>

#include "collision.h"

namespace ramiel {

    using ColliderPair = std::pair<Collider*, Collider*>;

    using BroadCollisionFn = std::function<
        std::vector<ColliderPair>(const std::vector<Collider*>& colliders)
    >;

    BroadCollisionFn bruteForce();

}
