#include "triangle.h"

namespace ramiel {

    TriInterpolate3d::TriInterpolate3d(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2
    ) {
        this->v1 = sizeView<3>(v1);
        x = sizeView<3>(v0 - v1);
        y = sizeView<3>(v2 - v1);
        z = normalize(cross(x, y));
        a = 1.0f / det(Mat3x3f{ x, y, z });
    }

    Vec3f TriInterpolate3d::operator()(const Vec4f& p) const {
        Vec3f vp = sizeView<3>(p) - v1;
        Vec3f weights;
        weights[0] = det(Mat3x3f{ vp, y, z }) * a;
        weights[2] = det(Mat3x3f{ x, vp, z }) * a;
        weights[1] = 1.0f - (weights[0] + weights[2]);
        return weights;
    }

}
