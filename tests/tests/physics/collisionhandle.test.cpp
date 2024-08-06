#include <ramiel/test.h>
#include <ramiel/physics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(CollisionSphereSphere) {
    SphereCollider o1(0.5f, Kinematics(Vec3f(), Vec3f(), { -3.3f,  6.9f, 2.7f }), 1.8f);
    SphereCollider o2(0.85, Kinematics({ 0.25f, 1.0f, 0.52f }, Vec3f(), { -2.8f, -3.3f, 7.9f }), 2.5f);

    Vec3f o1_pos_expected = { -0.02837f, -0.13793f, -0.05067f };
    Vec3f o2_pos_expected = {  0.27042f,  1.09931f,  0.55648f };
    Vec3f o1_vel_expected = { -4.96318f, -1.18612f, -0.27063f };
    Vec3f o2_vel_expected = { -1.60251,   2.522f,   10.0389f  };

    Collide_Sph_Sph handler(&o1, &o2);
    RAMIEL_TEST_ASSERT(handler.detect());
    handler.resolve();

    RAMIEL_TEST_ASSERT(equal(o1.pos, o1_pos_expected));
    RAMIEL_TEST_ASSERT(equal(o2.pos, o2_pos_expected));
    RAMIEL_TEST_ASSERT(equal(o1.pv, o1_vel_expected));
    RAMIEL_TEST_ASSERT(equal(o2.pv, o2_vel_expected));
}


RAMIEL_TEST_ADD(CollisionAabbAabb) {
    AabbCollider o1({ 1.0f, 0.8f, 1.1f }, Kinematics({ -1.1f, -0.3f, -0.6f }, Vec3f(), {  9.1f, -6.5f, -8.4f }), 0.9f);
    AabbCollider o2({ 1.1f, 1.4f, 0.6f }, Kinematics({ -1.5f,  0.5f, -1.0f }, Vec3f(), { -8.6f,  2.6f, -8.1f }), 1.0f);

    Vec3f o1_pos_expected = { -3.73158f, -0.3f, -0.6f };
    Vec3f o2_pos_expected = {  0.86842f,  0.5f, -1.0f };
    Vec3f o1_vel_expected = { -9.53158f, -6.5f, -8.4f };
    Vec3f o2_vel_expected = {  8.16842f,  2.6f, -8.1f };

    Collide_Aabb_Aabb handler(&o1, &o2);
    RAMIEL_TEST_ASSERT(handler.detect());
    handler.resolve();

    RAMIEL_TEST_ASSERT(equal(o1.pos, o1_pos_expected));
    RAMIEL_TEST_ASSERT(equal(o2.pos, o2_pos_expected));
    RAMIEL_TEST_ASSERT(equal(o1.pv, o1_vel_expected));
    RAMIEL_TEST_ASSERT(equal(o2.pv, o2_vel_expected));
}
