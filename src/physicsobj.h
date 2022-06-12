#pragma once

#include "vec.h"

namespace ramiel {

    class SphereCollider;


    class PhysicsObj {
    protected:
        // dynamics
        float dynamic;
        Vec3f pos;
        Vec3f rot;
        Vec3f posVel;
        Vec3f rotVel;
        Vec3f posAcc;
        Vec3f rotAcc;

        // collision
        float mass;

    public:
        PhysicsObj(
            float dynamic = false,
            Vec3f pos = vec3f_0,
            Vec3f rot = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0,
            float mass = 1.0f
        );
        virtual ~PhysicsObj();

        // dynamics
        void step();

        // collision
        virtual void collideWith(PhysicsObj* other);
        virtual void collideWith(SphereCollider* other);

        // getters
        inline const Vec3f& getPos() const { return pos; }
        inline const Vec3f& getRot() const { return rot; }
    };


    class SphereCollider : public PhysicsObj {
        float hbxrad;
    public:
        SphereCollider(
            float dynamic = false,
            Vec3f pos = vec3f_0,
            Vec3f rot = vec3f_0,
            Vec3f posVel = vec3f_0,
            Vec3f rotVel = vec3f_0,
            Vec3f posAcc = vec3f_0,
            Vec3f rotAcc = vec3f_0,
            float mass = 1.0f,
            float hbxrad = 0.5f
        );
        virtual ~SphereCollider();

        virtual void collideWith(PhysicsObj* other) override;
        virtual void collideWith(SphereCollider* other) override;

        friend void collideSphereSphere(SphereCollider& o1, SphereCollider& o2);
    };

}
