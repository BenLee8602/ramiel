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
            crv[i] = crv[i] * (1.0f - t) + crv[i + 1] * t;
        }
        return crvSolve(crv, deg - 1, t);
    }

}

