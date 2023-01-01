#include "catch2/catch2.hpp"

#include <light.h>
using namespace ramiel;


TEST_CASE("get directional light", "[light][get][dirlight]") {
    Light_Dir light({ 42, 174, 183 }, 1.0f, { -2, 2, 6 });
    Vec3f pos = { 6, 5, 2 };
    Vec3f normal = getNormalized({ 9, -4, -1 });
    Vec3f res = light.getLight(pos, normal, 10, 0.1f);
    Vec3f expected = { 20.4673f, 84.793f, 89.1788f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get pointlight", "[light][get][pointlight]") {
    Light_Pt light({ 198, 133, 66 }, 1.0f, { -2, -4, -1 }, 1.0f);
    Vec3f pos = { 0, -4, -2 };
    Vec3f normal = getNormalized({ -5, 1, 3 });
    Vec3f res = light.getLight(pos, normal, 64, 0.8f);
    Vec3f expected = { 32.4294f, 21.7834f, 10.8098f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get spotlight", "[light][get][spotlight]") {
    Light_Sp light({ 126, 215, 181 }, 1.0f, vec3f_0, { 0.0f, 0.0f, 1.0f }, 0.1f, 30.0f, 50.0f);
    Vec3f pos = { 1, -1, 4 };
    Vec3f normal = getNormalized({ 0, 0, -2 });
    Vec3f res = light.getLight(pos, normal, 36, 0.3f);
    Vec3f expected = { 2.23275f, 3.80986f, 3.20737f };
    REQUIRE(res.equals(expected));
}
