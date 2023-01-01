#include "catch2/catch2.hpp"

#include <rotation.h>
using namespace ramiel;


TEST_CASE("rotation set", "[rotation]") {
    Rotation rot;
    rot.set({ 5.9f, -9.0f, -2.1f });
    Vec3f sinExpected = { -0.373877f, -0.412118f, -0.863209f };
    Vec3f cosExpected = {  0.927478f, -0.91113f,  -0.504846f };
    REQUIRE(rot.sin().equals(sinExpected));
    REQUIRE(rot.cos().equals(cosExpected));
}


TEST_CASE("rotation rotate", "[rotation]") {
    Rotation rot = Vec3f{ -7.1f, 3.1f, -2.2f };
    Vec3f expected = { -2.72329f, -0.470847f, -0.248994f };
    REQUIRE(rot.rotate({ -1.5f, 2.3f, 0.4f }).equals(expected));
}
