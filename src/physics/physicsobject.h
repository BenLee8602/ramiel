#pragma once

#include <ramiel/math.h>

namespace ramiel {

    struct PhysicsObject {
        Vec3f pos;
        Vec3f rot;

        inline Mat4x4f getTransform() const {
            return matmat(translate(pos), rotate(rot));
        }
    };


    struct Kinematics : public PhysicsObject {
        Vec3f pv;
        Vec3f rv;
        Vec3f pa;
        Vec3f ra;

        inline void step(float dtime) {
            pv  += pa * dtime;
            rv  += ra * dtime;
            pos += pv * dtime;
            rot += rv * dtime;
        }
    };

}
