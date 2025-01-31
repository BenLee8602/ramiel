#pragma once

#include <functional>
#include <ramiel/math.h>

namespace ramiel {

    class Constraint;
    class Collider;

    using CollisionDetector = std::function<
        Constraint*(Collider*, Collider*)
    >;

}
