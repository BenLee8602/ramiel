#pragma once

#include "vec.h"

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
        PhysicsObj(
            bool dynamic = false,
            Vec3f pos = vec3f_0,
            Vec3f rot = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0
        );
        virtual ~PhysicsObj();

        void step();

        inline const Vec3f& getPos() const { return pos; }
        inline const Vec3f& getRot() const { return rot; }
    };


    class SphereCollider;


    class Collider : public PhysicsObj {
    #ifdef RAMIEL_TEST
    public:
    #else
    protected:
    #endif
        float mass;
    public:
        Collider(
            bool dynamic = false,
            Vec3f pos = vec3f_0,
            Vec3f rot = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0,
            float mass = 1.0f
        );
        virtual ~Collider();

        virtual void collideWith(Collider* other);
        virtual void collideWith(SphereCollider* other);
    };


    class SphereCollider : public Collider {
        float hbxrad;
    public:
        SphereCollider(
            bool dynamic = false,
            Vec3f pos = vec3f_0,
            Vec3f rot = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0,
            float mass = 1.0f,
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

        friend void collideSphereSphere(SphereCollider& o1, SphereCollider& o2);
    };

}
