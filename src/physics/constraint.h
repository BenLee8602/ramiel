#pragma once

#include <ramiel/math.h>

namespace ramiel {

    class Constraint {
    public:
        virtual void solve(float dt) = 0;
    };

    class Particle;
    class RigidBody;


    class DistanceConstraint : public Constraint {
    public:
        DistanceConstraint(
            float l0,
            float a,
            Particle* e1,
            Particle* e2
        );
        virtual void solve(float dt) override;

        float l0;
        float a;
        Particle* e1;
        Particle* e2;
    };


    class RopeConstraint : public Constraint {
    public:
        RopeConstraint(
            float l0,
            float a,
            RigidBody* e1,
            const Vec3f& r1,
            RigidBody* e2,
            const Vec3f& r2
        );
        virtual void solve(float dt) override;

        float l0;
        float a;

        RigidBody* e1;
        Vec3f r1;

        RigidBody* e2;
        Vec3f r2;
    };

}
