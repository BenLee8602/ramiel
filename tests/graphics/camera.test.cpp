#include <catch2/catch2.hpp>
#include <ramiel/graphics.h>
using namespace ramiel;


TEST_CASE("camera buffer size", "[camera]") {
    Camera camera;
    camera.setRes({ 10, 10 });
    REQUIRE(camera.getRes() == Vec2u{ 10, 10 });
    REQUIRE(camera.getBufferSize() == 100);
}


TEST_CASE("reset buffers", "[camera]") {
    Camera camera;
    camera.setRes({ 2, 1 });
    camera.backgroundColor = { 255, 0, 255 };
    camera.zfar = 10.0f;
    camera.resetBuffers();
    
    auto color = camera.getColorBuffer();
    auto depth = camera.getDepthBuffer();
    REQUIRE(color[0] == Vec3f{ 255, 0, 255 });
    REQUIRE(color[1] == Vec3f{ 255, 0, 255 });
    REQUIRE(depth[0] == 10.0f);
    REQUIRE(depth[1] == 10.0f);
}


TEST_CASE("camera coordinates", "[camera]") {
    Camera camera;
    Vec3f in;
    Vec3f expected;

    in = { -4.8f, 2.3f, 7.6f };
    expected = { -4.8f, 2.3f, 7.6f };
    camera.rot = { 0.0f, 0.0f, 0.0f };
    camera.pos = { 0.0f, 0.0f, 0.0f };
    camera.resetBuffers();
    REQUIRE(equal(camera.getCameraCoord(in), expected));

    in = { 9.9f, -5.3f, -9.7f };
    expected = { 10.1742f, 6.55266f, 8.58715f };
    camera.rot = { -5.4f, -1.3f, -4.5f };
    camera.pos = { 0.0f, 0.0f, 0.0f };
    camera.resetBuffers();
    REQUIRE(equal(camera.getCameraCoord(in), expected));

    in = { -2.1f, 6.8f, 0.5f };
    expected = { -7.9f, 15.3f, 2.1f };
    camera.rot = { 0.0f, 0.0f, 0.0f };
    camera.pos = { 5.8f, -8.5f, -1.6f };
    camera.resetBuffers();
    REQUIRE(equal(camera.getCameraCoord(in), expected));

    in = { 2.5f, -6.6f, -3.1f };
    expected = { -2.05688f, 5.63915f, 12.0108f };
    camera.rot = { -9.3f, 5.6f, 3.8f };
    camera.pos = { -8.7f, -6.9f, 4.3f };
    camera.resetBuffers();
    REQUIRE(equal(camera.getCameraCoord(in), expected));
}

TEST_CASE("screen coordinates", "[camera]") {
    Camera camera;
    Vec3f in;
    Vec2f expected;

    camera.setRes({ 720, 480 });
    camera.setFov(60);
    in = { -4.8f, 2.3f, 7.6f };
    expected = { -34, 428 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    camera.setRes({ 1280, 720 });
    camera.setFov(75);
    in = { 10.1f, 6.5f, 8.5f };
    expected = { 1631, 997 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    camera.setRes({ 1920, 1080 });
    camera.setFov(90);
    in = { -7.9f, 15.3f, 2.1f };
    expected = { -2653, 7536 };
    REQUIRE(camera.getScreenCoord(in) == expected);

    camera.setRes({ 2560, 1440 });
    camera.setFov(120);
    in = { -2.0f, 5.6f, 12.0f };
    expected = { 1156, 1065 };
    REQUIRE(camera.getScreenCoord(in) == expected);
}


TEST_CASE("clamp color buffer", "[camera]") {
    Camera camera;
    camera.setRes({ 2, 1 });
    auto color = camera.getColorBuffer();
    color[0] = { 255, 0, 255 };
    color[1] = { 300, 255, 100 };
    camera.clampColorBuffer();
    REQUIRE(color[0] == Vec3f{ 255, 0, 255 });
    REQUIRE(color[1] == Vec3f{ 255, 255, 100 });
}


TEST_CASE("get frame in rgb format", "[camera]") {
    uint8_t* rgb = new uint8_t[6]();

    Camera camera;
    camera.setRes({ 2, 1 });
    auto color = camera.getColorBuffer();
    color[0] = { 10, 3, 204 };
    color[1] = { 122, 99, 255 };
    camera.getFrameRGB(rgb);

    REQUIRE(rgb[0] == 10);
    REQUIRE(rgb[1] == 3);
    REQUIRE(rgb[2] == 204);
    REQUIRE(rgb[3] == 122);
    REQUIRE(rgb[4] == 99);
    REQUIRE(rgb[5] == 255);

    delete[] rgb;
}


TEST_CASE("get frame in dec format", "[camera]") {
    int* dec = new int[2]();

    Camera camera;
    camera.setRes({ 2, 1 });
    auto color = camera.getColorBuffer();
    color[0] = { 10, 204, 3 };
    color[1] = { 122, 99, 255 };
    camera.getFrameDEC(dec);
    
    REQUIRE(dec[0] == 707587);
    REQUIRE(dec[1] == 8020991);

    delete[] dec;
}
