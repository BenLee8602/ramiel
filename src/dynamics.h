#pragma once

#include "transform.h"

namespace ramiel {

    struct Dynamics : public Transform {
        Vec3f posVel;
        Vec3f rotVel;
        Vec3f posAcc;
        Vec3f rotAcc;

        Dynamics(
            Vec3f pos    = vec3f_0,
            Vec3f rot    = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0
        ) :
            Transform(pos, rot),
            posVel(posVel),
            rotVel(rotVel),
            posAcc(posAcc),
            rotAcc(rotAcc)
        {}
        Dynamics(
            Transform transform,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0
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
