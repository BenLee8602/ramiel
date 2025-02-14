#pragma once

#include <typeindex>
#include <ramiel/math.h>

namespace ramiel {

    class Collider {
    public:
        using Type = std::type_index;
        virtual Type getColliderType() const = 0;
    };

    class Particle;
    class RigidBody;


    class ParticleCollider : public Collider {
    public:
        virtual Type getColliderType() const override;
        virtual ~ParticleCollider() {}
        ParticleCollider(Particle* e);
        Particle* e;
    };

    class PlaneCollider : public Collider {
    public:
        virtual Type getColliderType() const override;
        virtual ~PlaneCollider() {}
        PlaneCollider(Vec3f n, float d);
        Vec3f n;
        float d;
    };

    class SphereCollider : public Collider {
    public:
        virtual Type getColliderType() const override;
        virtual ~SphereCollider() {}
        SphereCollider(RigidBody* e, float r);
        RigidBody* e;
        float r;
    };

    class BoxCollider : public Collider {
    public:
        virtual Type getColliderType() const override;
        virtual ~BoxCollider() {}
        BoxCollider(RigidBody* e, Vec3f size);
        RigidBody* e;
        Vec3f size;
    };

}
