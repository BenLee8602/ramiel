#include "catch2/catch2.hpp"
#include <texture.h>
using namespace bl;

TEST_CASE("texture size", "[texture]") {
    Texture texture("../data/colortest.png");
    Vec2 expected = { 640, 480 };
    REQUIRE(texture.getSize() == expected);
}

TEST_CASE("texture get", "[texture]") {
    Texture texture("../data/colortest.png");

    float n = 254.0f / 255.0f;
    Vec3f white   = { n, n, n };
    Vec3f yellow  = { n, n, 0 };
    Vec3f cyan    = { 0, n, n };
    Vec3f green   = { 0, n, 0 };
    Vec3f magenta = { n, 0, n };
    Vec3f red     = { n, 0, 0 };
    Vec3f blue    = { 0, 0, n };
    Vec3f black   = { 0, 0, 0 };

    REQUIRE(texture.get({  1.0f / 16.0f,  1.0f / 16.0f }) == white);
    REQUIRE(texture.get({  3.0f / 16.0f,  3.0f / 16.0f }) == yellow);
    REQUIRE(texture.get({  5.0f / 16.0f,  5.0f / 16.0f }) == cyan);
    REQUIRE(texture.get({  7.0f / 16.0f,  7.0f / 16.0f }) == green);
    REQUIRE(texture.get({  9.0f / 16.0f,  9.0f / 16.0f }) == magenta);
    REQUIRE(texture.get({ 11.0f / 16.0f, 11.0f / 16.0f }) == red);
    REQUIRE(texture.get({ 13.0f / 16.0f, 13.0f / 16.0f }) == blue);
    REQUIRE(texture.get({ 15.0f / 16.0f, 15.0f / 16.0f }) == black);
}
