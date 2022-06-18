#include "catch2/catch2.hpp"

#define RAMIEL_TEST
#include <collision.h>
using namespace ramiel;


TEST_CASE("sphere-sphere", "[collision][sphere]") {
    SphereCollider o1(true, vec3f_0, vec3f_0, { -3.3f, 6.9f, 2.7f }, vec3f_0, vec3f_0, vec3f_0, 1.8, 0.5);
    SphereCollider o2(true, { 0.25, 1, 0.52 }, vec3f_0, { -2.8f, -3.3f, 7.9f }, vec3f_0, vec3f_0, vec3f_0, 2.5, 0.85);

    Vec3f o1_pos_expected = { -0.02837f, -0.13793f, -0.05067f };
    Vec3f o2_pos_expected = {  0.27042f,  1.09931f,  0.55648f };
    Vec3f o1_vel_expected = { -4.96318f, -1.18612f, -0.27063f };
    Vec3f o2_vel_expected = { -1.60251,   2.522f,   10.0389f  };
    Approx totalMomentum = Approx(
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel)
    ).epsilon(0.1f);

    collideSphereSphere(o1, o2);

    REQUIRE(o1.pos.equals(o1_pos_expected));
    REQUIRE(o2.pos.equals(o2_pos_expected));
    REQUIRE(o1.posVel.equals(o1_vel_expected));
    REQUIRE(o2.posVel.equals(o2_vel_expected));
    REQUIRE( // conservation of momentum
        o1.mass * getMagnitude(o1.posVel) +
        o2.mass * getMagnitude(o2.posVel) == totalMomentum
    );
}
