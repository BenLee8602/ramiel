#include "catch2/catch2.hpp"

#include <vector>
#include "../src/effects.h"
using namespace ramiel;


TEST_CASE("ldr", "[effects]") {
    std::vector<Vec3f> pixels = {
        { 255.0f, 25.0f, 25.0f },
        { 100.0f, 0.0f, 25500.0f },
        { 25000.0f, 25.0f, 25.0f }
    };
    std::vector<float> depth;

    ldr({ 0, 0 }, 3, pixels.begin(), depth.begin());

    std::vector<Vec3> pixels_expected = {
        { 255, 25, 25 },
        { 9643, 15701, 25500 },
        { 25000, 12397, 12397 }
    };
    REQUIRE(std::equal(
        pixels.begin(), pixels.end(),
        pixels_expected.begin(),
        [](const Vec3f& acc, const Vec3& exp) { return exp == (Vec3)acc; }
    ));
}


TEST_CASE("hdr", "[effects]") {
    std::vector<Vec3f> pixels = {
        { 100.0f, 25.0f, 25.0f },
        { 100.0f, 2550.0f, 100.0f },
        { 300.0f, 255.0f, 255.0f }
    };
    std::vector<float> depth;

    hdr({ 0, 0 }, 3, pixels.begin(), depth.begin());

    std::vector<Vec3> pixels_expected = {
        {  86,  21,  21 },
        {  13, 351,  13 },
        { 150, 127, 127 }
    };
    REQUIRE(std::equal(
        pixels.begin(), pixels.end(),
        pixels_expected.begin(),
        [](const Vec3f& acc, const Vec3& exp) { return exp == (Vec3)acc; }
    ));
}


TEST_CASE("fog", "[effects]") {
    std::vector<Vec3f> pixels = { vec3f_255, vec3f_255, vec3f_255, vec3f_255, vec3f_255 };
    std::vector<float> depth  = { 0.5f, 1.0f, 1.5f, 2.0f, 2.5f };

    fog<0, 0, 0, 1, 2>({ 0, 0 }, 5, pixels.begin(), depth.begin());

    std::vector<Vec3f> pixels_expected = {
        vec3f_255, vec3f_255,
        { 127.5f, 127.5f, 127.5f },
        vec3f_0, vec3f_0
    };
    REQUIRE(std::equal(
        pixels.begin(), pixels.end(),
        pixels_expected.begin(),
        [](const Vec3f& p1, const Vec3f& p2) { return p1.equals(p2); }
    ));
}
