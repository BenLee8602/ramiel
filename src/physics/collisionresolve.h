#pragma once

#include "constraint.h"
#include "entity.h"

namespace ramiel {
    
    class CollisionConstraint_PP : public Constraint {
    public:
        CollisionConstraint_PP(
            Particle* e1,
            Particle* e2
        );
        virtual void solve(float dt) override;
    private:
        Particle* e1;
        Particle* e2;
    };

    class CollisionConstraint_RR : public Constraint {
    public:
        CollisionConstraint_RR(
            RigidBody* e1,
            RigidBody* e2,
            Vec3f p1,
            Vec3f p2
        );
        virtual void solve(float dt) override;
    private:
        RigidBody* e1;
        RigidBody* e2;
        Vec3f p1;
        Vec3f p2;
    };

    class CollisionConstraint_PR : public Constraint {
    public:
        CollisionConstraint_PR(
            Particle* e1,
            RigidBody* e2,
            Vec3f p2
        );
        virtual void solve(float dt) override;
    private:
        Particle* e1;
        RigidBody* e2;
        Vec3f p2;
    };

    class CollisionConstraint_P : public Constraint {
    public:
        CollisionConstraint_P(
            Vec3f e1,
            Particle* e2
        );
        virtual void solve(float dt) override;
    private:
        Vec3f e1;
        Particle* e2;
    };
    
    class CollisionConstraint_R : public Constraint {
    public:
        CollisionConstraint_R(
            Vec3f e1,
            RigidBody* e2,
            Vec3f p
        );
        virtual void solve(float dt) override;
    private:
        CollisionConstraint_PR constraint;
        Particle e1;
    };


}
