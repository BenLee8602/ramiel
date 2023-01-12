#include "catch2/catch2.hpp"

#include <dynamics.h>
using namespace ramiel;

TEST_CASE("dynamics step", "[dynamics]") {
    Dynamics dynamics(
        { -1.3f,  2.0f,  1.0f },
        { -8.9f, -0.4f, -3.2f },
        { -3.6f,  8.4f,  4.8f },
        {  2.7f,  8.6f, -4.7f },
        {  4.0f, -7.9f,  0.1f },
        { -7.8f,  2.6f,  1.0f }
    );
    float dtime;
    Vec3f pos_expected;
    Vec3f rot_expected;
    Vec3f posVel_expected;
    Vec3f rotVel_expected;

    dtime = 9.4f;
    pos_expected = { 318.3f, -617.084f, 54.956f };
    rot_expected = { -672.728f, 310.176f, 40.98f };
    posVel_expected = { 34.0f, -65.86f, 5.74f };
    rotVel_expected = { -70.62f, 33.04f, 4.7f };
    dynamics.step(dtime);
    REQUIRE(dynamics.pos.equals(pos_expected));
    REQUIRE(dynamics.rot.get().equals(rot_expected));
    REQUIRE(dynamics.posVel.equals(posVel_expected));
    REQUIRE(dynamics.rotVel.equals(rotVel_expected));

    dtime = 1.3f;
    pos_expected = { 369.26f, -716.053f, 62.587f };
    rot_expected = { -777.716f, 357.522f, 48.78f };
    posVel_expected = { 39.2f, -76.13f, 5.87f };
    rotVel_expected = { -80.76f, 36.42f, 6.0f };
    dynamics.step(dtime);
    REQUIRE(dynamics.pos.equals(pos_expected));
    REQUIRE(dynamics.rot.get().equals(rot_expected));
    REQUIRE(dynamics.posVel.equals(posVel_expected));
    REQUIRE(dynamics.rotVel.equals(rotVel_expected));
}
