#pragma once

#include <functional>
#include "collider.h"

namespace ramiel {

    struct CollisionHandler {
        virtual bool detect() = 0;
        virtual void resolve() = 0;
    };


    class Collide_Sph_Sph : public CollisionHandler {
        void resolveIC();
    public:
        SphereCollider* sphere1;
        SphereCollider* sphere2;
        float time;
        Collide_Sph_Sph(Collider* sphere1, Collider* sphere2);
        virtual bool detect() override;
        virtual void resolve() override;
    };


    class Collide_Aabb_Aabb : public CollisionHandler {
        void resolveIC();
    public:
        AabbCollider* box1;
        AabbCollider* box2;
        float time;
        size_t axis;
        Collide_Aabb_Aabb(Collider* box1, Collider* box2);
        virtual bool detect() override;
        virtual void resolve() override;
    };

}
