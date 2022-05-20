#include "catch2/catch2.hpp"

#include <physics.h>
#include <graphicsbl_p.h>
using namespace bl;


TEST_CASE("simulate movement", "[physics]") {
    GraphicsBL::dtime = 1.0f;
    Physics physics(vec3f_0, vec3f_0, false, 0.5f, 1.0f, true,
        {  5.9f,  2.8f,  3.1f },  // position velocity
        {  9.3f, -1.8f, -8.5f },  // position acceleration
        { -2.7f,  7.7f, -8.7f },  // rotation velocity
        {  7.1f,  2.2f, -5.7f }   // rotation acceleration
    );
    
    Vec3f pos_expected_1 = { 5.9f, 2.8f, 3.1f };
    Vec3f rot_expected_1 = { -2.7f, 7.7f, -8.7f };
    Vec3f posvel_expected_1 = { 15.2f, 1.0f, -5.4f };
    Vec3f rotvel_expected_1 = { 4.4f, 9.9f, -14.4f };
    physics.simulateMovement();
    REQUIRE(physics.pos.equals(pos_expected_1));
    REQUIRE(physics.rot.equals(rot_expected_1));
    REQUIRE(physics.posVel.equals(posvel_expected_1));
    REQUIRE(physics.rotVel.equals(rotvel_expected_1));

    Vec3f pos_expected_2 = { 21.1f, 3.8f, -2.3f };
    Vec3f rot_expected_2 = { 1.7f, 17.6f, -23.1f };
    Vec3f posvel_expected_2 = { 24.5f, -0.8f, -13.9f };
    Vec3f rotvel_expected_2 = { 11.5f, 12.1f, -20.1f };
    physics.simulateMovement();
    REQUIRE(physics.pos.equals(pos_expected_2));
    REQUIRE(physics.rot.equals(rot_expected_2));
    REQUIRE(physics.posVel.equals(posvel_expected_2));
    REQUIRE(physics.rotVel.equals(rotvel_expected_2));
}

TEST_CASE("simulate collision, objects too far", "[physics][collision]") {
    Physics p1({ -0.5, 0, 0 }, vec3f_0, false, 0.4f, 1.0f, true, { 1, 0, 0 });
    Physics p2({ 0.5, 0, 0 }, vec3f_0, true, 0.3f, 1.1f, true, { -0.5, 0, 0 });
    Vec3f p1_vel_expected = { 1, 0, 0 };
    Vec3f p2_vel_expected = { -0.5, 0, 0 };
    p1.simulateCollision(p2);
    REQUIRE(p1.posVel.equals(p1_vel_expected));
    REQUIRE(p2.posVel.equals(p2_vel_expected));
}

TEST_CASE("simulate collision, both moveable", "[physics][collision]") {
    Physics p1(vec3f_0, vec3f_0, true, 0.5f, 1.8f, true, { -3.3f, 6.9f, 2.7f });
    Physics p2({ 0.25, 1, 0.52 }, vec3f_0, true, 0.85f, 2.5f, true, { -2.8f, -3.3f, 7.9f });

    Approx totalMomentum = Approx(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel)).epsilon(0.1f);
    Vec3f p1_vel_expected = { -4.90757f, 0.469712f, -0.643749f };
    Vec3f p2_vel_expected = { -1.64255f, 1.32981f, 10.3075f };

    p1.simulateCollision(p2);

    REQUIRE(p1.posVel.equals(p1_vel_expected));
    REQUIRE(p2.posVel.equals(p2_vel_expected));
    REQUIRE(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel) == totalMomentum); // conservation of momentum
}

TEST_CASE("simulate collision, this moveable", "[physics][collision]") {
    Physics p1(vec3f_0, vec3f_0, true, 0.6f, 2.0f, true, { 2.4f, 0.0f, 1.3f });
    Physics p2({ -0.7f, 0.1f, -0.2f }, vec3f_0, true, 0.4f, 1.4f);

    Approx totalMomentum = Approx(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel)).epsilon(0.1f);
    Vec3f p1_vel_expected = { -2.62963f, 0.718519f, -0.137037f };
    Vec3f p2_vel_expected = { 0.0f, 0.0f, 0.0f };

    p1.simulateCollision(p2);

    REQUIRE(p1.posVel.equals(p1_vel_expected));
    REQUIRE(p2.posVel.equals(p2_vel_expected));
    REQUIRE(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel) == totalMomentum); // conservation of momentum
}

TEST_CASE("simulate collision, other moveable", "[physics][collision]") {
    Physics p1(vec3f_0, vec3f_0, true, 0.2f, 0.35f);
    Physics p2({ 0.05f, -0.3f, -0.15f }, vec3f_0, true, 0.5f, 2.1f, true, { 0.1f, 3.1f, 0.25f });

    Approx totalMomentum = Approx(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel)).epsilon(0.1f);
    Vec3f p1_vel_expected = { 0.0f, 0.0f, 0.0f };
    Vec3f p2_vel_expected = { 0.936956f, -1.92174f, -2.26087f };

    p1.simulateCollision(p2);

    REQUIRE(p1.posVel.equals(p1_vel_expected));
    REQUIRE(p2.posVel.equals(p2_vel_expected));
    REQUIRE(p1.mass * getMagnitude(p1.posVel) + p2.mass * getMagnitude(p2.posVel) == totalMomentum); // conservation of momentum
}
