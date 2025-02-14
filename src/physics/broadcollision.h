#pragma once

#include <functional>
#include <vector>

namespace ramiel {

    class Collider;

    using ColliderPair = std::pair<Collider*, Collider*>;

    using BroadCollisionFn = std::function<
        std::vector<ColliderPair>(const std::vector<Collider*>& colliders)
    >;

    BroadCollisionFn bruteForce();

}
