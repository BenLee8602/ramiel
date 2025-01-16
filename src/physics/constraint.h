#pragma once

namespace ramiel {

    class Constraint {
    public:
        virtual void solve(float dt) = 0;
    };

    class Particle;


    class DistanceConstraint : public Constraint {
    public:
        DistanceConstraint(float l0, float a, Particle* e1, Particle* e2);
        virtual void solve(float dt) override;

        float l0;
        float a;
        Particle* e1;
        Particle* e2;
    };


    class GroundConstraint : public Constraint {
    public:
        GroundConstraint(Particle* e);
        virtual void solve(float dt) override;

        Particle* e;
    };

}
