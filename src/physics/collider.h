#pragma once

#include <typeindex>
#include <ramiel/math.h>
#include "dynamics.h"

namespace ramiel {

    typedef std::type_index ColliderType;


    struct Collider : public Dynamics {
        Collider(
            Dynamics dynamics = Dynamics()
        ) : Dynamics(dynamics) {}

        virtual ColliderType getType() const = 0;
        virtual ~Collider() {}
    };


    struct SphereCollider : public Collider {
        float mass;
        float hbxrad;

        SphereCollider(
            float hbxrad,
            Dynamics dynamics = Dynamics(),
            float mass = 0.0f
        ) : 
            hbxrad(hbxrad),
            Collider(dynamics),
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
            Dynamics dynamics = Dynamics(),
            float mass = 0.0f
        ) : 
            size(size),
            Collider(dynamics),
            mass(mass)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(AabbCollider));
        };
    };

}
