#pragma once

#include <ramiel/math.h>
#include "physicsobject.h"

namespace ramiel {

    struct SphereCollider : public Collider {
        float mass;
        float hbxrad;

        SphereCollider(
            float hbxrad,
            Kinematics kinematics = Kinematics(),
            float mass = 0.0f
        ) : 
            hbxrad(hbxrad),
            Collider(kinematics),
            mass(mass)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(SphereCollider));
        };
    };


    struct AabbCollider : public Collider {
        float mass;
        Vec3f size;

        AabbCollider(
            Vec3f size,
            Kinematics kinematics = Kinematics(),
            float mass = 0.0f
        ) : 
            size(size),
            Collider(kinematics),
            mass(mass)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(AabbCollider));
        };
    };

}
