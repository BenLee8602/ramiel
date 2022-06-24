#include "catch2/catch2.hpp"
#include <mesh.h>
using namespace ramiel;

#ifndef ramiel_TEST_DATA_DIR
#define ramiel_TEST_DATA_DIR "."
#endif


TEST_CASE("mesh constructor", "[mesh]") {
    Mesh mesh(std::string(ramiel_TEST_DATA_DIR) + "/cube_withquads.obj", 1.1f, { -8.5f, -6.8f, 0.4f }, { -3.9f, 7.9f, -9.6f });
    std::vector<Vec3f> v_pos_expected = {
        { -9.92874f, -7.38299f,  1.19043f  },
        { -8.82991f, -7.34819f,  1.22716f  },
        { -9.91992f, -6.72844f,  0.306415f },
        { -8.82109f, -6.69363f,  0.343149f },
        { -9.87891f, -8.26637f,  0.536851f },
        { -8.78008f, -8.23156f,  0.573585f },
        { -9.87009f, -7.61181f, -0.347164f },
        { -8.77126f, -7.57701f, -0.310431f }
    };
    std::vector<Vec3f> v_normal_expected = {
        { -0.83740f,  0.05909f,  0.54339f },
        {  0.29744f,  0.14923f,  0.94301f },
        { -0.35783f,  0.92153f, -0.15079f },
        {  0.80041f,  0.59661f, -0.05825f },
        { -0.30813f, -0.94262f,  0.12853f },
        {  0.83085f, -0.54494f,  0.11279f },
        { -0.79387f, -0.11076f, -0.59792f },
        {  0.36852f, -0.12813f, -0.92074f }
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
        { 8.89438f, 9.62485f, 4.20835f },
        { 8.14875f, 9.34254f, 4.14249f },
        { 9.1737f,  8.87656f, 4.25352f },
        { 8.42807f, 8.59425f, 4.18765f },
        { 8.97193f, 9.60575f, 3.41235f },
        { 8.2263f,  9.32344f, 3.34648f },
        { 9.25125f, 8.85746f, 3.45751f },
        { 8.50562f, 8.57515f, 3.39165f }
    };
    mesh.getVPos(v_pos, 0.8f, { 8.7f, 9.1f, 3.8f }, { 1.8f, -1.2f, -1.3f });
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
        { -0.81327f,  0.35183f, -0.46345f },
        {  0.08932f, -0.27932f, -0.95603f },
        { -0.73988f, -0.63159f,  0.23162f },
        {  0.41488f, -0.90965f,  0.01985f },
        {  0.17293f,  0.97314f, -0.15194f },
        {  0.97387f,  0.07304f, -0.21503f },
        { -0.57548f,  0.48477f,  0.65864f },
        {  0.47763f, -0.06222f,  0.87635f }
    };
    mesh.getVNormal(v_normal, { 1.8f, 7.3f, -5.9f });
    REQUIRE(std::equal(
        v_normal.begin(), v_normal.end(),
        v_normal_expected.begin(),
        [](const Vec3f& v1, const Vec3f& v2) { return v1.equals(v2); }
    ));
}
