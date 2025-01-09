#pragma once

namespace ramiel {

    class Constraint {
    public:
        virtual void solve(float dt) = 0;
    };

    class PhysicsEntity;


    class DistanceConstraint : public Constraint {
    public:
        DistanceConstraint(float l0, float a, PhysicsEntity* e1, PhysicsEntity* e2);
        virtual void solve(float dt) override;

        float l0;
        float a;
        PhysicsEntity* e1;
        PhysicsEntity* e2;
    };


    class GroundConstraint : public Constraint {
    public:
        GroundConstraint(PhysicsEntity* e);
        virtual void solve(float dt) override;

        PhysicsEntity* e;
    };

}
