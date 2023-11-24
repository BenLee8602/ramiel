#pragma once

#include <ramiel/math.h>

namespace ramiel {

    struct Dynamics : public Transform {
        Vec3f posVel;
        Vec3f rotVel;
        Vec3f posAcc;
        Vec3f rotAcc;

        Dynamics(
            Vec3f pos    = Vec3f(),
            Vec3f rot    = Vec3f(),
            Vec3f posVel = Vec3f(),
            Vec3f rotVel = Vec3f(),
            Vec3f posAcc = Vec3f(),
            Vec3f rotAcc = Vec3f()
        ) :
            Transform(pos, rot),
            posVel(posVel),
            rotVel(rotVel),
            posAcc(posAcc),
            rotAcc(rotAcc)
        {}
        Dynamics(
            Transform transform,
            Vec3f posVel = Vec3f(),
            Vec3f rotVel = Vec3f(),
            Vec3f posAcc = Vec3f(),
            Vec3f rotAcc = Vec3f()
        ) :
            Transform(transform),
            posVel(posVel),
            rotVel(rotVel),
            posAcc(posAcc),
            rotAcc(rotAcc)
        {}

        inline void step(float dtime) {
            posVel += posAcc * dtime;
            rotVel += rotAcc * dtime;
            pos    += posVel * dtime;
            rot    += rotVel * dtime;
        }
    };

}
