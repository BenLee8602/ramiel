#include "catch2/catch2.hpp"

#define BL_TEST
#include <light.h>
using namespace bl;


TEST_CASE("construct directional light", "[light][construct][dirlight]") {
    Light_Dir light1(vec3f_255, vec3f_0);
    Vec3f dir1_expected = { 0.0f, 0.0f, -1.0f };
    REQUIRE(light1.dir.equals(dir1_expected));

    Light_Dir light2(vec3f_255, { 11, -54, 52 });
    Vec3f dir2_expected = { 0.14517f, -0.71268f, 0.68629f };
    REQUIRE(light2.dir.equals(dir2_expected));
}

TEST_CASE("construct pointlight", "[light][construct][pointlight]") {
    Light_Pt light(vec3f_255, vec3f_0, -0.2f);
    Approx falloff_expected = Approx(1.0f).epsilon(0.0001f);
    REQUIRE(light.falloff == falloff_expected);
}

TEST_CASE("construct spotlight", "[light][construct][spotlight]") {
    Light_Sp light(vec3f_255, vec3f_0, vec3f_0, -0.7f);
    Vec3f dir_expected = { 0.0f, 0.0f, 1.0f };
    Approx falloff_expected = Approx(0.1f).epsilon(0.0001f);
    Approx width_expected = Approx(0.91667f).epsilon(0.0001f);
    REQUIRE(light.dir.equals(dir_expected));
    REQUIRE(light.falloff == falloff_expected);
    REQUIRE(light.width == width_expected);

    light = Light_Sp(vec3f_255, vec3f_0, { -13, -49, 64 });
    dir_expected = { -0.15922f, -0.60015f, 0.78388f };
    REQUIRE(light.dir.equals(dir_expected));
}

TEST_CASE("get directional light", "[light][get][dirlight]") {
    Light_Dir light({ 42, 174, 183 }, { -2, 2, 6 });
    Vec3f pos = { 6, 5, 2 };
    Vec3f normal = { -9, -4, -1 };
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 25.327f, 104.926f, 110.353f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get pointlight", "[light][get][pointlight]") {
    Light_Pt light({ 198, 133, 66 }, { -2, -4, -1 });
    Vec3f pos = { 0, -4, -2 };
    Vec3f normal = { -5, 1, 3 };
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 191.855f, 128.872f, 63.9515f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get spotlight", "[light][get][spotlight]") {
    Light_Sp light({ 126, 215, 181 });
    Vec3f pos = { 1, -1, 4 };
    Vec3f normal = { 0, 0, -2 };
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 4.46534f, 7.61943f, 6.4145f };
    REQUIRE(res.equals(expected));
}
