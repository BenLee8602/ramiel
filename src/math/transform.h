#pragma once

#include "rotation.h"

namespace ramiel {

    struct Transform {
        Vec3f pos;
        Rotation rot;
        float scale;

        Transform(
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
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
