#pragma once

#include "vec.h"

namespace ramiel {

    class Rotation {
        Vec3f rot;
        Vec3f sin;
        Vec3f cos;
    public:
        Rotation();
        Rotation(Vec3f rot);
        Rotation& operator=(const Vec3f& rot);
        Rotation& operator+=(const Vec3f& rot);
        void set(const Vec3f& rot);
        operator bool() const;
        operator Vec3f() const;
        const Vec3f& get() const;
        const Vec3f& getSin() const;
        const Vec3f& getCos() const;
        Vec3f rotate(Vec3f in) const;
    };

}
