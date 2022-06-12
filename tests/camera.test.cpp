#include "catch2/catch2.hpp"

#define RAMIEL_TEST
#include <camera.h>
#include <ramiel.h>
using namespace ramiel;


TEST_CASE("camera rot trig calculations", "[camera]") {
    Camera camera;
    camera.rotate(7, -4, 3);
    camera.calcTrigValues();

    Vec3f sin_expected = { 0.656987f, 0.756802f, 0.14112f };
    Vec3f cos_expected = { 0.753902f, -0.653644f, -0.989992f };
    REQUIRE(camera.sin.equals(sin_expected));
    REQUIRE(camera.cos.equals(cos_expected));
}

TEST_CASE("set camera fov", "[camera]") {
    Camera camera;
    graphics::setBufferSize({ 10, 10 });

    camera.setFov(75);
    REQUIRE(camera.focalLen == Approx(12.0f).epsilon(0.001f));

    camera.setFov(90);
    REQUIRE(camera.focalLen == Approx(10.0f).epsilon(0.001f));

    camera.setFov(120);
    REQUIRE(camera.focalLen == Approx(7.5f).epsilon(0.001f));
}

TEST_CASE("camera coordinates", "[camera]") {
    Camera camera;
    Vec3f in;
    Vec3f expected;

    in = { -4.8f, 2.3f, 7.6f };
    expected = { -4.8f, 2.3f, 7.6f };
    camera.setrot({ 0.0f, 0.0f, 0.0f });
    camera.setpos({ 0.0f, 0.0f, 0.0f });
    camera.calcTrigValues();
    REQUIRE(camera.getCameraCoord(in).equals(expected));

    in = { 9.9f, -5.3f, -9.7f };
    expected = { 10.1742f, 6.55266f, 8.58715f };
    camera.setrot({ -5.4f, -1.3f, -4.5f });
    camera.setpos({ 0.0f, 0.0f, 0.0f });
    camera.calcTrigValues();
    REQUIRE(camera.getCameraCoord(in).equals(expected));

    in = { -2.1f, 6.8f, 0.5f };
    expected = { -7.9f, 15.3f, 2.1f };
    camera.setrot({ 0.0f, 0.0f, 0.0f });
    camera.setpos({ 5.8f, -8.5f, -1.6f });
    camera.calcTrigValues();
    REQUIRE(camera.getCameraCoord(in).equals(expected));

    in = { 2.5f, -6.6f, -3.1f };
    expected = { -2.05688f, 5.63915f, 12.0108f };
    camera.setrot({ -9.3f, 5.6f, 3.8f });
    camera.setpos({ -8.7f, -6.9f, 4.3f });
    camera.calcTrigValues();
    REQUIRE(camera.getCameraCoord(in).equals(expected));
}

TEST_CASE("screen coordinates", "[camera]") {
    Camera camera;
    Vec3f in;
    Vec2 expected;

    graphics::setBufferSize({ 720, 480 });
    camera.setFov(60);
    in = { -4.8f, 2.3f, 7.6f };
    expected = { -322, 566 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    graphics::setBufferSize({ 1280, 720 });
    camera.setFov(75);
    in = { 10.1f, 6.5f, 8.5f };
    expected = { 2465, 1534 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    graphics::setBufferSize({ 1920, 1080 });
    camera.setFov(90);
    in = { -7.9f, 15.3f, 2.1f };
    expected = { -6262, 14528 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    graphics::setBufferSize({ 2560, 1440 });
    camera.setFov(120);
    in = { -2.0f, 5.6f, 12.0f };
    expected = { 960, 1616 };
    REQUIRE(camera.getScreenCoord(in) == expected);
}
