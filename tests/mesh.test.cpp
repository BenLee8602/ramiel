#include "catch2/catch2.hpp"
#include <mesh.h>
using namespace ramiel;

#ifndef ramiel_TEST_DATA_DIR
#define ramiel_TEST_DATA_DIR "."
#endif


TEST_CASE("mesh constructor", "[mesh]") {
    Mesh mesh(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj", { -8.5f, -6.8f, 0.4f }, { -3.9f, 7.9f, -9.6f });
    std::vector<Vec3f> v_pos_expected = {
        { -9.02613f, -6.71181f, 1.08221f },
        { -8.02719f, -6.68017f, 1.1156f },
        { -9.01811f, -6.11676f, 0.278559f },
        { -8.01917f, -6.08512f, 0.311954f },
        { -8.98083f, -7.51488f, 0.488046f },
        { -7.98189f, -7.48324f, 0.521441f },
        { -8.97281f, -6.91983f, -0.315604f },
        { -7.97387f, -6.88819f, -0.28221f }
    };
    std::vector<Vec3f> v_normal_expected = {
        { -0.837399f, 0.0590911f, 0.543389f },
        { 0.297436f, 0.149227f, 0.943008f },
        { -0.357833f, 0.92153f, -0.15079f },
        { 0.800413f, 0.596611f, -0.0582563f },
        { -0.308128f, -0.942623f, 0.128527f },
        { 0.830851f, -0.544945f, 0.112789f },
        { -0.793866f, -0.110757f, -0.597922f },
        { 0.368525f, -0.128134f, -0.920745f }
    };
    REQUIRE(std::equal(
        mesh.getVNormal().begin(), mesh.getVNormal().end(),
        v_normal_expected.begin(),
        [](const Vec3f& v1, const Vec3f& v2) { return v1.equals(v2); }
    ));
}

TEST_CASE("get vpos", "[mesh]") {
    Mesh mesh(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj");
    std::vector<Vec3f> v_pos;
    std::vector<Vec3f> v_pos_expected = {
        { 8.94298f, 9.75606f, 4.31044f },
        { 8.01094f, 9.40318f, 4.22811f },
        { 9.29213f, 8.8207f, 4.3669f },
        { 8.36009f, 8.46782f, 4.28457f },
        { 9.03991f, 9.73218f, 3.31543f },
        { 8.10787f, 9.3793f, 3.2331f },
        { 9.38906f, 8.79682f, 3.37189f },
        { 8.45702f, 8.44394f, 3.28956f }
    };
    mesh.getVPos(v_pos, { 8.7f, 9.1f, 3.8f }, { 1.8f, -1.2f, -1.3f });
    REQUIRE(std::equal(
        v_pos.begin(), v_pos.end(),
        v_pos_expected.begin(),
        [](const Vec3f& v1, const Vec3f& v2) { return v1.equals(v2); }
    ));
}

TEST_CASE("get vnormal", "[mesh]") {
    Mesh mesh(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj");
    std::vector<Vec3f> v_normal;
    std::vector<Vec3f> v_normal_expected = {
        { -0.813276f, 0.351836f, -0.463459f },
        { 0.0893206f, -0.27932f, -0.956035f },
        { -0.739888f, -0.631598f, 0.231625f },
        { 0.414886f, -0.909657f, 0.0198503f },
        { 0.17293f, 0.973144f, -0.151941f },
        { 0.973871f, 0.073043f, -0.215035f },
        { -0.575482f, 0.484778f, 0.658644f },
        { 0.477637f, -0.0622262f, 0.876351f },
    };
    mesh.getVNormal(v_normal, { 1.8f, 7.3f, -5.9f });
    REQUIRE(std::equal(
        v_normal.begin(), v_normal.end(),
        v_normal_expected.begin(),
        [](const Vec3f& v1, const Vec3f& v2) { return v1.equals(v2); }
    ));
}
