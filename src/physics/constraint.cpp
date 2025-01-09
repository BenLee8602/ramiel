#include <cassert>

#include "constraint.h"
#include "entity.h"

namespace ramiel {

    DistanceConstraint::DistanceConstraint(
        float l0,
        float a,
        PhysicsEntity* e1,
        PhysicsEntity* e2
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


    GroundConstraint::GroundConstraint(PhysicsEntity* e)
        : e(e)
    {
        assert(e);
    }

    void GroundConstraint::solve(float dt) {
        e->pos[Y] = std::max(0.0f, e->pos[Y]);
    }

}
