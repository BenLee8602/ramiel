#pragma once

#include <ramiel/math.h>

namespace ramiel {

    Vec3f inertiaSphere(float m, float r);
    Vec3f inertiaBox(float m, Vec3f x);
    Vec3f inertiaCube(float m, float x);

}
