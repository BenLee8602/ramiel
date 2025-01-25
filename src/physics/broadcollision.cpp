#include "broadcollision.h"
using namespace ramiel;

namespace {

    std::vector<ColliderPair> bruteForceImpl(const std::vector<Collider*>& colliders) {
        const size_t n = colliders.size();
        std::vector<ColliderPair> out;
        out.reserve(n * (n - 1) / 2);

        for (size_t i = 0; i < colliders.size(); i++) {
            for (size_t j = i + 1; j < colliders.size(); j++) {
                out.emplace_back(colliders[i], colliders[j]);
            }
        }

        return out;
    }

}

namespace ramiel {

    BroadCollisionFn bruteForce() {
        return bruteForceImpl;
    }

}
