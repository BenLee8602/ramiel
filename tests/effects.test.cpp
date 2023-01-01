#include "catch2/catch2.hpp"

#include <vector>
#include "../src/effects.h"
using namespace ramiel;


TEST_CASE("hdr", "[effects]") {
    Camera camera;
    camera.res({ 3, 1 });
    auto color = camera.getColorBuffer();
    color[0] = { 100,   25,  25 };
    color[1] = { 100, 2550, 100 };
    color[2] = { 300,  255, 255 };

    hdr(camera);

    std::vector<Vec3> pixels_expected = {
        {  86,  21,  21 },
        {  13, 351,  13 },
        { 150, 127, 127 }
    };
    REQUIRE(std::equal(
        color, color + 3,
        pixels_expected.begin(),
        [](const Vec3f& acc, const Vec3& exp) { return exp == (Vec3)acc; }
    ));
}


TEST_CASE("fog", "[effects]") {
    Camera camera;
    camera.res({ 5, 1 });
    auto color = camera.getColorBuffer();
    auto depth = camera.getDepthBuffer();
    for (size_t i = 0; i < 5; ++i) {
        color[i] = vec3f_255;
        depth[i] = (float)(i + 1) * 0.5f;
    }

    fog<0, 0, 0, 1, 2>(camera);

    std::vector<Vec3f> pixels_expected = {
        vec3f_255, vec3f_255,
        { 127.5f, 127.5f, 127.5f },
        vec3f_0, vec3f_0
    };
    REQUIRE(std::equal(
        color, color + 5,
        pixels_expected.begin(),
        [](const Vec3f& p1, const Vec3f& p2) { return p1.equals(p2); }
    ));
}
