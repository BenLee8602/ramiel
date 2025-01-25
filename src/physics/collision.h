#pragma once

#include <typeindex>
#include <functional>

#include <ramiel/math.h>

namespace ramiel {

    class Collider {
    public:
        using Type = std::type_index;
        virtual Type getColliderType() const = 0;
    };

    class Constraint;

    using CollisionDetector = std::function<
        Constraint*(Collider*, Collider*)
    >;

}
