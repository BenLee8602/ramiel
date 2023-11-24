#pragma once

#include "rotation.h"

namespace ramiel {

    struct Transform {
        Vec3f pos;
        Rotation rot;
        float scale;

        Transform(
            Vec3f pos = Vec3f(),
            Rotation rot = Vec3f(),
            float scale = 1.0f
        ) :
            pos(pos),
            rot(rot),
            scale(scale)
        {}

        inline Vec3f operator()(const Vec3f& in) const {
            return rot.rotate(in) * scale + pos;
        }
    };

}
