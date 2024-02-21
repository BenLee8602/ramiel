#include <catch2/catch2.hpp>
#include <ramiel/physics.h>
using namespace ramiel;


bool equal(const CollisionHandler& f1, const CollisionHandler& f2) {
    if (!f1 && !f2) return true;
    if (!f1 || !f2) return false;
    if (f1.target_type() != f2.target_type()) return false;
    size_t t1 = (size_t)f1.target<CollisionHandler>();
    size_t t2 = (size_t)f2.target<CollisionHandler>();
    return t1 == t2;
}


TEST_CASE("collision dispatch", "[collisiondispatch]") {
    SphereCollider o1(0.5f, Dynamics(Vec3f(), Vec3f(), { -3.3f,  6.9f, 2.7f }), true, 1.8f);
    SphereCollider o2(0.85, Dynamics({ 0.25f, 1.0f, 0.52f }, Vec3f(), { -2.8f, -3.3f, 7.9f }), true, 2.5f);

    Vec3f o1_pos_expected = { -0.02837f, -0.13793f, -0.05067f };
    Vec3f o2_pos_expected = {  0.27042f,  1.09931f,  0.55648f };

    dispatchCollision(&o1, &o2);

    REQUIRE(equal(o1.pos, o1_pos_expected));
    REQUIRE(equal(o2.pos, o2_pos_expected));
}


TEST_CASE("get collision handler", "[collisiondispatch]") {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    CollisionHandler expected = collideAabbAabb;
    CollisionHandler actual = getCollisionHandler(aabbCollider, aabbCollider);
    REQUIRE(equal(expected, actual));
}


TEST_CASE("set collision handler", "[collisiondispatch]") {
    ColliderType sphereCollider = ColliderType(typeid(SphereCollider));
    CollisionHandler myHandler = [](Collider* c1, Collider* c2) { return; };
    setCollisionHandler(sphereCollider, sphereCollider, myHandler);
    CollisionHandler res = getCollisionHandler(sphereCollider, sphereCollider);
    REQUIRE(equal(res, myHandler));
}


TEST_CASE("delete collision handler", "[collisiondispatch]") {
    ColliderType aabbCollider = ColliderType(typeid(AabbCollider));
    deleteCollisionHandler(aabbCollider, aabbCollider);
    REQUIRE(getCollisionHandler(aabbCollider, aabbCollider) == nullptr);
}
