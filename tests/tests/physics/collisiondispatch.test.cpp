#include <ramiel/test.h>
#include <ramiel/physics.h>
using namespace ramiel;


bool equal(const CollisionHandlerFactory& f1, const CollisionHandlerFactory& f2) {
    if (!f1 && !f2) return true;
    if (!f1 || !f2) return false;
    if (f1.target_type() != f2.target_type()) return false;
    size_t t1 = (size_t)f1.target<CollisionHandlerFactory>();
    size_t t2 = (size_t)f2.target<CollisionHandlerFactory>();
    return t1 == t2;
}


RAMIEL_TEST_ADD(CollisionDispatchGetHandlerByCollider) {
    SphereCollider collider(1.0f);
    CollisionHandler_P handler = getCollisionHandler(&collider, &collider);
    RAMIEL_TEST_ASSERT(dynamic_cast<Collide_Sph_Sph*>(handler.get()));
}


RAMIEL_TEST_ADD(CollisionDispatchGetHandlerByType) {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    CollisionHandlerFactory expected = collide<Collide_Aabb_Aabb>;
    CollisionHandlerFactory actual = getCollisionHandler(aabbCollider, aabbCollider);
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(CollisionDispatchSetHandler) {
    ColliderType sphereCollider = ColliderType(typeid(SphereCollider));
    CollisionHandlerFactory myHandler = [](Collider* c1, Collider* c2) { return CollisionHandler_P(); };
    setCollisionHandler(sphereCollider, sphereCollider, myHandler);
    CollisionHandlerFactory res = getCollisionHandler(sphereCollider, sphereCollider);
    RAMIEL_TEST_ASSERT(equal(res, myHandler));
}


RAMIEL_TEST_ADD(CollisionDispatchDeleteHandler) {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    deleteCollisionHandler(aabbCollider, aabbCollider);
    RAMIEL_TEST_ASSERT(getCollisionHandler(aabbCollider, aabbCollider) == nullptr);
}
