#include "interpolate.h"
#include "mat.h"

namespace ramiel {

    TriInterpolate2d::TriInterpolate2d(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2
    ) {
        this->v1 = sizeView<2>(v1);
        x = sizeView<2>(v0 - v1);
        y = sizeView<2>(v2 - v1);
        a = 1.0f / det(Mat2x2f{ x, y });
    }

    Vec3f TriInterpolate2d::operator()(const Vec2f& p) const {
        Vec2f vp = p - v1;
        Vec3f weights;
        weights[0] = (vp[0] * y[1] - vp[1] * y[0]) * a;
        weights[2] = (x[0] * vp[1] - x[1] * vp[0]) * a;
        weights[1] = 1.0f - (weights[0] + weights[2]);
        return weights;
    }


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
