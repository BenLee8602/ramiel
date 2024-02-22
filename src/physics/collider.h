#pragma once

#include <typeindex>
#include <ramiel/math.h>
#include "dynamics.h"

namespace ramiel {

    typedef std::type_index ColliderType;


    struct Collider : public Dynamics {
        bool responsive;
        float mass;

        Collider(
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) :
            Dynamics(dynamics),
            responsive(responsive),
            mass(mass)
        {}

        virtual ColliderType getType() const = 0;
        virtual ~Collider() {}
    };


    struct SphereCollider : public Collider {
        float hbxrad;

        SphereCollider(
            float hbxrad,
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(dynamics, responsive, mass),
            hbxrad(hbxrad)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(SphereCollider));
        };
    };


    struct AabbCollider : public Collider {
        Vec3f size;

        AabbCollider(
            Vec3f size,
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(dynamics, responsive, mass),
            size(size)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(AabbCollider));
        };
    };


    struct ObbCollider : public Collider {
        Vec3f size;

        ObbCollider(
            Vec3f size,
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(dynamics, responsive, mass),
            size(size)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(ObbCollider));
        };
    };


    struct MeshCollider : public Collider {
        Mesh<MeshVertex>& mesh;

        MeshCollider(
            Mesh<MeshVertex>& mesh,
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(dynamics, responsive, mass),
            mesh(mesh)
        {}

        virtual ColliderType getType() const {
            return ColliderType(typeid(MeshCollider));
        };
    };

}
