#include "collision.h"

namespace ramiel {

    void SphereCollider::collideWith(Collider*       other) { other->collideWith(this); }
    void SphereCollider::collideWith(SphereCollider* other) { collide(*this, *other); }
    void SphereCollider::collideWith(AabbCollider*   other) { collide(*this, *other); }
    void SphereCollider::collideWith(ObbCollider*    other) { collide(*this, *other); }
    void SphereCollider::collideWith(MeshCollider*   other) { collide(*this, *other); }

    void AabbCollider::collideWith(Collider*       other) { other->collideWith(this); }
    void AabbCollider::collideWith(SphereCollider* other) { collide(*other, *this); }
    void AabbCollider::collideWith(AabbCollider*   other) { collide(*this, *other); }
    void AabbCollider::collideWith(ObbCollider*    other) { collide(*this, *other); }
    void AabbCollider::collideWith(MeshCollider*   other) { collide(*this, *other); }

    void ObbCollider::collideWith(Collider*       other) { other->collideWith(this); }
    void ObbCollider::collideWith(SphereCollider* other) { collide(*other, *this); }
    void ObbCollider::collideWith(AabbCollider*   other) { collide(*other, *this); }
    void ObbCollider::collideWith(ObbCollider*    other) { collide(*this, *other); }
    void ObbCollider::collideWith(MeshCollider*   other) { collide(*this, *other); }

    void MeshCollider::collideWith(Collider*       other) { other->collideWith(this); }
    void MeshCollider::collideWith(SphereCollider* other) { collide(*other, *this); }
    void MeshCollider::collideWith(AabbCollider*   other) { collide(*other, *this); }
    void MeshCollider::collideWith(ObbCollider*    other) { collide(*other, *this); }
    void MeshCollider::collideWith(MeshCollider*   other) { collide(*this, *other); }

}
