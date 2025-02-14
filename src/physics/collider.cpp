#include <cassert>
#include "collider.h"

namespace ramiel {

    Collider::Type ParticleCollider::getColliderType() const {
        return typeid(ParticleCollider);
    }

    ParticleCollider::ParticleCollider(Particle* e)
        : e(e)
    {
        assert(e);
    }


    Collider::Type PlaneCollider::getColliderType() const {
        return typeid(PlaneCollider);
    }

    PlaneCollider::PlaneCollider(Vec3f n, float d)
        : n(normalize(n))
        , d(d)
    {
        assert(d >= 0.0f);
    }


    Collider::Type SphereCollider::getColliderType() const {
        return typeid(SphereCollider);
    }

    SphereCollider::SphereCollider(RigidBody* e, float r)
        : e(e)
        , r(r)
    {
        assert(e);
        assert(r > 0.0f);
    }


    Collider::Type BoxCollider::getColliderType() const {
        return typeid(BoxCollider);
    }

    BoxCollider::BoxCollider(RigidBody* e, Vec3f size)
        : e(e)
        , size(size)
    {
        assert(e);
        assert(size[X] > 0.0f);
        assert(size[Y] > 0.0f);
        assert(size[Z] > 0.0f);
    }


}
