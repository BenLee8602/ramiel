#include <catch2/catch2.hpp>
#include <ramiel/graphics.h>
using namespace ramiel;


TEST_CASE("camera buffer size", "[camera]") {
    setRes({ 10, 10 });
    REQUIRE(getRes() == Vec2u{ 10, 10 });
    REQUIRE(getBufferSize() == 100);
}


TEST_CASE("camera coordinates", "[camera]") {
    Vec4f in;
    Vec4f expected;

    in = { -4.8f, 2.3f, 7.6f, 1.0f };
    expected = { -4.8f, 2.3f, 7.6f, 1.0f };
    setRot({ 0.0f, 0.0f, 0.0f });
    setPos({ 0.0f, 0.0f, 0.0f });
    REQUIRE(equal(getCameraCoord(in), expected));

    in = { 9.9f, -5.3f, -9.7f, 1.0f };
    expected = { 10.1742f, 6.55266f, 8.58715f, 1.0f };
    setRot({ -5.4f, -1.3f, -4.5f });
    setPos({ 0.0f, 0.0f, 0.0f });
    REQUIRE(equal(getCameraCoord(in), expected));

    in = { -2.1f, 6.8f, 0.5f, 1.0f };
    expected = { -7.9f, 15.3f, 2.1f, 1.0f };
    setRot({ 0.0f, 0.0f, 0.0f });
    setPos({ 5.8f, -8.5f, -1.6f });
    REQUIRE(equal(getCameraCoord(in), expected));

    in = { 2.5f, -6.6f, -3.1f, 1.0f };
    expected = { -2.05688f, 5.63915f, 12.0108f, 1.0f };
    setRot({ -9.3f, 5.6f, 3.8f });
    setPos({ -8.7f, -6.9f, 4.3f });
    REQUIRE(equal(getCameraCoord(in), expected));
}

TEST_CASE("screen coordinates", "[camera]") {
    Vec4f in;
    Vec4f expected;

    setRes({ 720, 480 });
    setFov(60);
    in = { -4.8f, 2.3f, 7.6f, 1.0f };
    expected = { -34, 428, 7.6f, 1.0f };
    REQUIRE(getScreenCoord(in) == expected);

    setRes({ 1280, 720 });
    setFov(75);
    in = { 10.1f, 6.5f, 8.5f, 1.0f };
    expected = { 1631, 997, 8.5f, 1.0f };
    REQUIRE(getScreenCoord(in) == expected);

    setRes({ 1920, 1080 });
    setFov(90);
    in = { -7.9f, 15.3f, 2.1f, 1.0f };
    expected = { -2653, 7536, 2.1f, 1.0f };
    REQUIRE(getScreenCoord(in) == expected);

    setRes({ 2560, 1440 });
    setFov(120);
    in = { -2.0f, 5.6f, 12.0f, 1.0f };
    expected = { 1156, 1065, 12.0f, 1.0f };
    REQUIRE(getScreenCoord(in) == expected);
}
