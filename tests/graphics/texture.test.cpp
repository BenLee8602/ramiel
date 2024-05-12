#include <catch2/catch2.hpp>
#include <ramiel/file.h>
#include <ramiel/graphics.h>
using namespace ramiel;


TEST_CASE("texture size", "[texture]") {
    Texture texture(std::string(ramiel_TEST_DATA_DIR) + "/colortest.png");
    Vec2 expected = { 640, 480 };
    REQUIRE(texture.getSize() == expected);
}

TEST_CASE("texture get color", "[texture]") {
    Texture texture(std::string(ramiel_TEST_DATA_DIR) + "/colortest.png", rgb1);

    const float n = 254.0f / 255.0f;
    const Vec3f white   = { n, n, n };
    const Vec3f yellow  = { n, n, 0 };
    const Vec3f cyan    = { 0, n, n };
    const Vec3f green   = { 0, n, 0 };
    const Vec3f magenta = { n, 0, n };
    const Vec3f red     = { n, 0, 0 };
    const Vec3f blue    = { 0, 0, n };
    const Vec3f black   = { 0, 0, 0 };

    REQUIRE(equal(texture.get({  1.0f / 16.0f,  1.0f / 16.0f }), white));
    REQUIRE(equal(texture.get({  3.0f / 16.0f,  3.0f / 16.0f }), yellow));
    REQUIRE(equal(texture.get({  5.0f / 16.0f,  5.0f / 16.0f }), cyan));
    REQUIRE(equal(texture.get({  7.0f / 16.0f,  7.0f / 16.0f }), green));
    REQUIRE(equal(texture.get({  9.0f / 16.0f,  9.0f / 16.0f }), magenta));
    REQUIRE(equal(texture.get({ 11.0f / 16.0f, 11.0f / 16.0f }), red));
    REQUIRE(equal(texture.get({ 13.0f / 16.0f, 13.0f / 16.0f }), blue));
    REQUIRE(equal(texture.get({ 15.0f / 16.0f, 15.0f / 16.0f }), black));
}

TEST_CASE("texture get normal vector", "[texture]") {
    Texture texture(std::string(ramiel_TEST_DATA_DIR) + "/colortest.png", normalMap);

    const float n = 0.992157f;
    const Vec3f expected1 = { -1.0f, -1.0f, -1.0f };
    const Vec3f expected2 = { -1.0f,     n, -1.0f };
    const Vec3f expected3 = { -1.0f,     n,     n };
    const Vec3f expected4 = {     n,     n,     n };
    REQUIRE(equal(texture.get({ 15.0f / 16.0f, 0.5f }), expected1));
    REQUIRE(equal(texture.get({  7.0f / 16.0f, 0.5f }), expected2));
    REQUIRE(equal(texture.get({  5.0f / 16.0f, 0.5f }), expected3));
    REQUIRE(equal(texture.get({  1.0f / 16.0f, 0.5f }), expected4));
}
