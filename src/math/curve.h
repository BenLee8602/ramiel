#pragma once

#include <array>
#include "vec.h"

namespace ramiel {

    Vec2f crvSolve(Vec2f* crv, size_t deg, float t);
    Vec2f crvSolve(Vec2f p0, Vec2f p1, Vec2f p2, float t);

}

