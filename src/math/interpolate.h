#pragma once

#include "vec.h"

namespace ramiel {

    class TriInterpolate2d {
    public:
        TriInterpolate2d(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2);
        Vec3f operator()(const Vec2f& p) const;
    private:
        Vec2f v1;
        Vec2f x;
        Vec2f y;
        float a;
    };

    class TriInterpolate3d {
    public:
        TriInterpolate3d(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2);
        Vec3f operator()(const Vec4f& p) const;
    private:
        Vec3f v1;
        Vec3f x;
        Vec3f y;
        Vec3f z;
        float a;
    };

}
