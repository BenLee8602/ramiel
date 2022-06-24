#pragma once

#include "vec.h"
#include "mesh.h"

#define RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS \
bool dynamic = false,                   \
Vec3f pos = vec3f_0,                    \
Vec3f rot = vec3f_0,                    \
Vec3f posVel = vec3f_0,                 \
Vec3f rotVel = vec3f_0,                 \
Vec3f posAcc = vec3f_0,                 \
Vec3f rotAcc = vec3f_0                  \

#define RAMIEL_COLLIDER_ARGS float mass = 1.0f

namespace ramiel {

    class PhysicsObj {
    #ifdef RAMIEL_TEST
    public:
    #else
    protected:
    #endif
        bool dynamic;
        Vec3f pos;
        Vec3f rot;
        Vec3f posVel;
        Vec3f rotVel;
        Vec3f posAcc;
        Vec3f rotAcc;

    public:
        PhysicsObj(RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS);
        virtual ~PhysicsObj();

        void step();

        inline const Vec3f& getPos() const { return pos; }
        inline const Vec3f& getRot() const { return rot; }
    };


    class SphereCollider;
    class AabbCollider;
    class ObbCollider;
    class MeshCollider;


    class Collider : public PhysicsObj {
    #ifdef RAMIEL_TEST
    public:
    #else
    protected:
    #endif
        float mass;
    public:
        Collider(
            RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS,
            RAMIEL_COLLIDER_ARGS
        );
        virtual ~Collider();

        virtual void collideWith(Collider* other);
        virtual void collideWith(SphereCollider* other);
        virtual void collideWith(AabbCollider* other);
        virtual void collideWith(ObbCollider* other);
        virtual void collideWith(MeshCollider* other);
    };


    class SphereCollider : public Collider {
        float hbxrad;
    public:
        SphereCollider(
            RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS,
            RAMIEL_COLLIDER_ARGS,
            float hbxrad = 0.5f
        ) : 
            Collider(
                dynamic,
                pos, rot,
                posVel, rotVel,
                posAcc, rotAcc,
                mass
            ),
            hbxrad(hbxrad)
        {}

        virtual void collideWith(Collider* other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider* other) override;
        virtual void collideWith(ObbCollider* other) override;
        virtual void collideWith(MeshCollider* other) override;

        friend void collideSphereSphere(SphereCollider& o1, SphereCollider& o2);
        friend void collideSphereAabb(SphereCollider& sph, AabbCollider& box);
        friend void collideSphereObb(SphereCollider& sph, ObbCollider& box);
        friend void collideSphereMesh(SphereCollider& sph, MeshCollider& mesh);
    };


    class AabbCollider : public Collider {
        Vec3f size;
    public:
        AabbCollider(
            Vec3f size,
            RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS,
            RAMIEL_COLLIDER_ARGS
        ) : 
            Collider(
                dynamic,
                pos, rot,
                posVel, rotVel,
                posAcc, rotAcc,
                mass
            ),
            size(size)
        {}

        virtual void collideWith(Collider* other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider* other) override;
        virtual void collideWith(ObbCollider* other) override;
        virtual void collideWith(MeshCollider* other) override;
        
        friend void collideSphereAabb(SphereCollider& sph, AabbCollider& box);
        friend void collideAabbAabb(AabbCollider& o1, AabbCollider& o2);
        friend void collideAabbObb(AabbCollider& aa, ObbCollider& ori);
        friend void collideAabbMesh(AabbCollider& box, MeshCollider& mesh);
    };


    class ObbCollider : public Collider {
        // idk
    public:
        ObbCollider(
            // idk,
            RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS,
            RAMIEL_COLLIDER_ARGS
        );
        virtual void collideWith(Collider* other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider* other) override;
        virtual void collideWith(ObbCollider* other) override;
        virtual void collideWith(MeshCollider* other) override;

        friend void collideSphereObb(SphereCollider& sph, ObbCollider& box);
        friend void collideAabbObb(AabbCollider& aa, ObbCollider& ori);
        friend void collideObbObb(ObbCollider& o1, ObbCollider& o2);
        friend void collideObbMesh(ObbCollider& box, MeshCollider& mesh);
    };


    class MeshCollider : public Collider {
        Mesh* mesh;
    public:
        MeshCollider(
            Mesh* mesh,
            RAMIEL_PHYSICSOBJ_DYNAMICS_ARGS,
            RAMIEL_COLLIDER_ARGS
        );

        virtual void collideWith(Collider* other) override;
        virtual void collideWith(SphereCollider* other) override;
        virtual void collideWith(AabbCollider* other) override;
        virtual void collideWith(ObbCollider* other) override;
        virtual void collideWith(MeshCollider* other) override;

        friend void collideSphereMesh(SphereCollider& sph, MeshCollider& mesh);
        friend void collideAabbMesh(AabbCollider& box, MeshCollider& mesh);
        friend void collideObbMesh(ObbCollider& box, MeshCollider& mesh);
        friend void collideMeshMesh(MeshCollider& o1, MeshCollider& o2);
    };

}
