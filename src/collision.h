#pragma once

#include "physicsobject.h"

namespace ramiel {
    
    void collideSphereSphere(SphereCollider& o1, SphereCollider& o2);
    void collideSphereAabb(SphereCollider& sph, AabbCollider& box);
    void collideSphereObb(SphereCollider& sph, ObbCollider& box);
    void collideSphereMesh(SphereCollider& sph, MeshCollider& mesh);

    void collideAabbAabb(AabbCollider& o1, AabbCollider& o2);
    void collideAabbObb(AabbCollider& aa, ObbCollider& ori);
    void collideAabbMesh(AabbCollider& box, MeshCollider& mesh);

    void collideObbObb(ObbCollider& o1, ObbCollider& o2);
    void collideObbMesh(ObbCollider& box, MeshCollider& mesh);

    void collideMeshMesh(MeshCollider& o1, MeshCollider& o2);

}
