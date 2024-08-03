#include "triangle.h"

namespace ramiel {

    bool isFrontFacing(const Vec4f& v1, const Vec4f& v2, const Vec4f& v3) {
        return 0.0f > det(Mat3x3f{
            Vec3f{ v1[X], v1[Y], v1[Z] },
            Vec3f{ v2[X], v2[Y], v2[Z] },
            Vec3f{ v3[X], v3[Y], v3[Z] }
        });
    }

}
