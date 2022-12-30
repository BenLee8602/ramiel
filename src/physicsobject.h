#pragma once

#include "mesh.h"

namespace ramiel {

    struct PhysicsObject {
        Vec3f pos;
        Rotation rot;
        Vec3f posVel;
        Vec3f rotVel;
        Vec3f posAcc;
        Vec3f rotAcc;

        PhysicsObject(
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0
        ) :
            pos(pos),
            rot(rot),
            posVel(vec3f_0),
            rotVel(vec3f_0),
            posAcc(vec3f_0),
            rotAcc(vec3f_0)
        {}
        virtual ~PhysicsObject() {}

        void step(float dtime);
    };


    class SphereCollider;
    class AabbCollider;
    class ObbCollider;
    class MeshCollider;


    struct Collider : public PhysicsObject {
        bool responsive;
        float mass;

        Collider(
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
            bool responsive = true,
            float mass = 1.0f
        ) :
            PhysicsObject(pos, rot),
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
            float hbxrad = 0.5f,
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(pos, rot, responsive, mass),
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


    struct AabbCollider : public Collider {
        Vec3f size;

        AabbCollider(
            Vec3f size,
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(pos, rot, responsive, mass),
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


    struct ObbCollider : public Collider {
        Vec3f size;

        ObbCollider(
            Vec3f size,
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(pos, rot, responsive, mass),
            size(size)
        {}
        
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


    struct MeshCollider : public Collider {
        Mesh<Vertex_Mesh>& mesh;

        MeshCollider(
            Mesh<Vertex_Mesh>& mesh,
            Vec3f pos = vec3f_0,
            Rotation rot = vec3f_0,
            bool responsive = true,
            float mass = 1.0f
        ) : 
            Collider(pos, rot, responsive, mass),
            mesh(mesh)
        {}

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
