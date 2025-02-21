#include <cassert>
#include "collisionresolve.h"
using namespace ramiel;

namespace {

    float rigidWass(RigidBody* e, Vec3f r, Vec3f n) {
        if (e->wass == 0.0f) return 0.0f;
        Vec3f pn = qtnvec(qtninv(e->rot), cross(r, n));
        Vec3f w = pn * pn * e->woi;
        return w[X] + w[Y] + w[Z] + e->wass;
    }


    void rigidRotCorrection(RigidBody* e, Vec3f p, Vec3f n) {
        Quaternion drot{};

        Vec3f& dr3 = sizeView<3>(drot);
        dr3 = cross(p - e->pos, n);
        dr3 = qtnvec(qtninv(e->rot), dr3);
        dr3 *= e->woi;
        dr3 = qtnvec(e->rot, dr3);

        e->rot += qtnqtn(drot, e->rot) * 0.5f;
        e->rot = normalize(e->rot);
    }

}

namespace ramiel {

    CollisionConstraint_PP::CollisionConstraint_PP(
        Particle* e1,
        Particle* e2
    )
        : e1(e1)
        , e2(e2)
    {
        assert(e1);
        assert(e2);
    }


    void CollisionConstraint_PP::solve(float dt) {
        float w = e1->wass + e2->wass;
        if (w == 0.0f) return;

        Vec3f p = e2->pos - e1->pos;
        float c = mag(p);
        Vec3f grad = p / c;
        float lambda = -c / w;
        
        e1->pos -= grad * lambda * e1->wass;
        e2->pos += grad * lambda * e2->wass;
    }


    CollisionConstraint_RR::CollisionConstraint_RR(
        RigidBody* e1,
        RigidBody* e2,
        Vec3f p1,
        Vec3f p2
    )
        : e1(e1)
        , e2(e2)
        , p1(p1)
        , p2(p2)
    {
        assert(e1);
        assert(e2);
    }


    void CollisionConstraint_RR::solve(float dt) {
        Vec3f r1 = qtnvec(qtninv(e1->rot), p1 - e1->pos);
        Vec3f r2 = qtnvec(qtninv(e2->rot), p2 - e2->pos);

        Vec3f p = p2 - p1;
        float c = mag(p);
        Vec3f grad = p / c;

        float w = rigidWass(e1, r1, grad) + rigidWass(e2, r2, grad);
        if (w == 0.0f) return;

        float lambda = -c / w;
        Vec3f cor = grad * lambda;

        e1->pos -= cor * e1->wass;
        e2->pos += cor * e2->wass;
        
        rigidRotCorrection(e1, p1, -cor);
        rigidRotCorrection(e2, p2, cor);
    }


    CollisionConstraint_PR::CollisionConstraint_PR(
        Particle* e1,
        RigidBody* e2,
        Vec3f p2
    )
        : e1(e1)
        , e2(e2)
        , p2(p2)
    {
        assert(e1);
        assert(e2);
    }


    void CollisionConstraint_PR::solve(float dt) {
        Vec3f r2 = qtnvec(qtninv(e2->rot), p2 - e2->pos);

        Vec3f p = p2 - e1->pos;
        float c = mag(p);
        Vec3f grad = p / c;

        float w = e1->wass + rigidWass(e2, r2, grad);
        if (w == 0.0f) return;

        float lambda = -c / w;
        Vec3f cor = grad * lambda;

        e1->pos -= cor * e1->wass;
        e2->pos += cor * e2->wass;
        
        rigidRotCorrection(e2, p2, cor);
    }


    CollisionConstraint_P::CollisionConstraint_P(
        Vec3f e1,
        Particle* e2
    )
        : e1(e1)
        , e2(e2)
    {
        assert(e2);
    }


    void CollisionConstraint_P::solve(float dt) {
        e2->pos = e1;
    }


    CollisionConstraint_R::CollisionConstraint_R(
        Vec3f e1,
        RigidBody* e2,
        Vec3f p
    )
        : e1(e1, Vec3f{}, 0.0f)
        , constraint(&this->e1, e2, p)
    {
        assert(e2);
    }


    void CollisionConstraint_R::solve(float dt) {
        constraint.solve(dt);
    }

}
