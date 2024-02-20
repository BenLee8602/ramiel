#pragma once

#include <functional>
#include "collider.h"

namespace ramiel {

    void collide(SphereCollider& o1, SphereCollider& o2);
    void collide(SphereCollider& sph, AabbCollider& box);
    void collide(SphereCollider& sph, ObbCollider& box);
    void collide(SphereCollider& sph, MeshCollider& mesh);

    void collide(AabbCollider& o1, AabbCollider& o2);
    void collide(AabbCollider& aa, ObbCollider& ori);
    void collide(AabbCollider& box, MeshCollider& mesh);

    void collide(ObbCollider& o1, ObbCollider& o2);
    void collide(ObbCollider& box, MeshCollider& mesh);

    void collide(MeshCollider& o1, MeshCollider& o2);


    typedef std::function<void(Collider*, Collider*)> CollisionHandler;

    void collideSphereSphere(Collider* collider1, Collider* collider2);
    void collideAabbAabb(Collider* collider1, Collider* collider2);

}
