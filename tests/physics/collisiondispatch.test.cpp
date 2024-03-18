#include <catch2/catch2.hpp>
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


TEST_CASE("get collision handler, colliders", "[collisiondispatch]") {
    SphereCollider collider(1.0f);
    CollisionHandler_P handler = getCollisionHandler(&collider, &collider);
    REQUIRE(dynamic_cast<Collide_Sph_Sph*>(handler.get()));
}


TEST_CASE("get collision handler, types", "[collisiondispatch]") {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    CollisionHandlerFactory expected = collide<Collide_Aabb_Aabb>;
    CollisionHandlerFactory actual = getCollisionHandler(aabbCollider, aabbCollider);
    REQUIRE(equal(expected, actual));
}


TEST_CASE("set collision handler", "[collisiondispatch]") {
    ColliderType sphereCollider = ColliderType(typeid(SphereCollider));
    CollisionHandlerFactory myHandler = [](Collider* c1, Collider* c2) { return CollisionHandler_P(); };
    setCollisionHandler(sphereCollider, sphereCollider, myHandler);
    CollisionHandlerFactory res = getCollisionHandler(sphereCollider, sphereCollider);
    REQUIRE(equal(res, myHandler));
}


TEST_CASE("delete collision handler", "[collisiondispatch]") {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    deleteCollisionHandler(aabbCollider, aabbCollider);
    REQUIRE(getCollisionHandler(aabbCollider, aabbCollider) == nullptr);
}
