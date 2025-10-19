#include <cassert>
#include <vector>

#include "curve.h"

namespace ramiel {

    Vec2f crvSolve(Vec2f* crv, size_t deg, float t) {
        assert(crv);
        assert(t >= 0.0f);
        assert(t <= 1.0f);

        if (deg == 0) return crv[0];

        for (size_t i = 0; i < deg; i++) {
            crv[i] = crv[i] + (crv[i + 1] - crv[i]) * t;
        }
        return crvSolve(crv, deg - 1, t);
    }


    Vec2f crvSolve(Vec2f p0, Vec2f p1, Vec2f p2, float t) {
        assert(t >= 0.0f);
        assert(t <= 1.0f);

        Vec2f q0 = p0 + (p1 - p0) * t;
        Vec2f q1 = p1 + (p2 - p1) * t;

        return q0 + (q1 - q0) * t;
    }

}

