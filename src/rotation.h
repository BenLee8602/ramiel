#pragma once

#include "vec.h"

namespace ramiel {

    class Rotation {
        Vec3f rot;
        Vec3f sine;
        Vec3f cosine;
    public:
        Rotation();
        Rotation(Vec3f rot);
        Rotation& operator=(const Vec3f& rot);
        Rotation& operator+=(const Vec3f& rot);
        void set(const Vec3f& rot);
        float operator[](size_t i) const;
        operator bool() const;
        operator Vec3f() const;
        const Vec3f& get() const;
        const Vec3f& sin() const;
        const Vec3f& cos() const;
        Vec3f rotate(Vec3f in) const;
    };

}
