#include "inertia.h"
using namespace ramiel;

namespace {

    Vec3f invert(Vec3f in) {
        return {
            1.0f / in[X],
            1.0f / in[Y],
            1.0f / in[Z]
        };
    }

}

namespace ramiel {

    Vec3f inertiaSphere(float m, float r) {
        constexpr float a = 2.0f / 3.0f;
        float i = 1.0f / (a * m * r * r);
        return { i, i, i };
    }


    Vec3f inertiaBox(float m, Vec3f x) {
        m /= 12.0f;
        x *= x;
        return invert({
            m * (x[Y] + x[Z]),
            m * (x[X] + x[Z]),
            m * (x[X] + x[Y])
        });
    }


    Vec3f inertiaCube(float m, float x) {
        return inertiaBox(m, { x, x, x });
    }

}
