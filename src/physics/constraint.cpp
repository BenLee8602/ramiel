#include <cassert>

#include "constraint.h"
#include "entity.h"
using namespace ramiel;

namespace {

    float rigidWass(RigidBody* e, Vec3f p, Vec3f n) {
        if (e->wass == 0.0f) return 0.0f;
        Vec3f pn = qtnvec(qtninv(e->rot), cross(p, n));
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

    DistanceConstraint::DistanceConstraint(
        float l0,
        float a,
        Particle* e1,
        Particle* e2
    )
        : l0(l0)
        , a(a)        
        , e1(e1)
        , e2(e2)
    {
        assert(l0 >= 0.0f);
        assert(a >= 0.0f && a <= 1.0f);
        assert(e1);
        assert(e2);
    }

    void DistanceConstraint::solve(float dt) {
        Vec3f p = e2->pos - e1->pos;
        float l = mag(p);

        float c = l - l0;
        Vec3f grad = p / l;
        float lambda = -c / (e1->wass + e2->wass + (a / (dt * dt)));

        e1->pos -= grad * lambda * e1->wass;
        e2->pos += grad * lambda * e2->wass;
    }


    RopeConstraint::RopeConstraint(
        float l0,
        float a,
        RigidBody* e1,
        const Vec3f& r1,
        RigidBody* e2,
        const Vec3f& r2
    )
        : l0(l0)
        , a(a)
        , e1(e1)
        , r1(r1)
        , e2(e2)
        , r2(r2)
    {
        assert(l0 >= 0.0f);
        assert(a >= 0.0f && a <= 1.0f);
        assert(e1);
        assert(e2);
    }

    void RopeConstraint::solve(float dt) {
        Vec3f p1 = qtnvec(e1->rot, r1) + e1->pos;
        Vec3f p2 = qtnvec(e2->rot, r2) + e2->pos;

        Vec3f p = p2 - p1;
        float l = mag(p);
        if (l < l0) return;

        float c = l - l0;
        Vec3f grad = p / l;

        float w = rigidWass(e1, r1, grad) + rigidWass(e2, r2, grad);
        if (w == 0.0f) return;

        float lambda = -c / (w + (a / (dt * dt)));
        Vec3f cor = grad * lambda;

        e1->pos -= cor * e1->wass;
        e2->pos += cor * e2->wass;
        
        rigidRotCorrection(e1, p1, -cor);
        rigidRotCorrection(e2, p2, cor);
    }

}
