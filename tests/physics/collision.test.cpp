#include <catch2/catch2.hpp>
#define RAMIEL_TEST
#include <ramiel/physics.h>
using namespace ramiel;


TEST_CASE("sphere-sphere", "[collision][sphere]") {
    SphereCollider o1(0.5f, Dynamics(vec3f_0, vec3f_0, { -3.3f,  6.9f, 2.7f }), true, 1.8f);
    SphereCollider o2(0.85, Dynamics({ 0.25f, 1.0f, 0.52f }, vec3f_0, { -2.8f, -3.3f, 7.9f }), true, 2.5f);

    Vec3f o1_pos_expected = { -0.02837f, -0.13793f, -0.05067f };
    Vec3f o2_pos_expected = {  0.27042f,  1.09931f,  0.55648f };
    Vec3f o1_vel_expected = { -4.96318f, -1.18612f, -0.27063f };
    Vec3f o2_vel_expected = { -1.60251,   2.522f,   10.0389f  };
    Approx totalMomentum = Approx(
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel)
    ).epsilon(0.1f);

    collide(o1, o2);

    REQUIRE(o1.pos.equals(o1_pos_expected));
    REQUIRE(o2.pos.equals(o2_pos_expected));
    REQUIRE(o1.posVel.equals(o1_vel_expected));
    REQUIRE(o2.posVel.equals(o2_vel_expected));
    REQUIRE( // conservation of momentum
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel) == totalMomentum
    );
}


TEST_CASE("aabb-aabb", "[collision][aabb]") {
    AabbCollider o1({ 1.0f, 0.8f, 1.1f }, Dynamics({ -1.1f, -0.3f, -0.6f }, vec3f_0, {  9.1f, -6.5f, -8.4f }), true, 0.9f);
    AabbCollider o2({ 1.1f, 1.4f, 0.6f }, Dynamics({ -1.5f,  0.5f, -1.0f }, vec3f_0, { -8.6f,  2.6f, -8.1f }), true, 1.0f);

    Vec3f o1_pos_expected = { -3.73158f, -0.3f, -0.6f };
    Vec3f o2_pos_expected = {  0.86842f,  0.5f, -1.0f };
    Vec3f o1_vel_expected = { -9.53158f, -6.5f, -8.4f };
    Vec3f o2_vel_expected = {  8.16842f,  2.6f, -8.1f };
    Approx totalMomentum = Approx(
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel)
    ).epsilon(0.1f);

    collide(o1, o2);

    REQUIRE(o1.pos.equals(o1_pos_expected));
    REQUIRE(o2.pos.equals(o2_pos_expected));
    REQUIRE(o1.posVel.equals(o1_vel_expected));
    REQUIRE(o2.posVel.equals(o2_vel_expected));
    REQUIRE( // conservation of momentum
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel) == totalMomentum
    );
}
