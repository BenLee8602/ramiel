#pragma once

#include <ramiel/math.h>
#include "dynamics.h"

namespace ramiel {

    class SphereCollider;
    class AabbCollider;
    class ObbCollider;
    class MeshCollider;


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

        virtual ~Collider() {}

        virtual void collideWith(Collider* other) = 0;
        virtual void collideWith(SphereCollider* other) = 0;
        virtual void collideWith(AabbCollider* other) = 0;
        virtual void collideWith(ObbCollider* other) = 0;
        virtual void collideWith(MeshCollider* other) = 0;
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

        virtual void collideWith(Collider*       other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider*   other) override;
        virtual void collideWith(ObbCollider*    other) override;
        virtual void collideWith(MeshCollider*   other) override;
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

        virtual void collideWith(Collider*       other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider*   other) override;
        virtual void collideWith(ObbCollider*    other) override;
        virtual void collideWith(MeshCollider*   other) override;
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

        virtual void collideWith(Collider*       other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider*   other) override;
        virtual void collideWith(ObbCollider*    other) override;
        virtual void collideWith(MeshCollider*   other) override;
    };


    struct MeshCollider : public Collider {
        Mesh<Vertex_Mesh>& mesh;

        MeshCollider(
            Mesh<Vertex_Mesh>& mesh,
            Dynamics dynamics = Dynamics(),
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(dynamics, responsive, mass),
            mesh(mesh)
        {}

        virtual void collideWith(Collider*       other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider*   other) override;
        virtual void collideWith(ObbCollider*    other) override;
        virtual void collideWith(MeshCollider*   other) override;
    };

}
