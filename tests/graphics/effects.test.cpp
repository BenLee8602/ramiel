#include <catch2/catch2.hpp>
#include <vector>
#include <ramiel/graphics.h>
using namespace ramiel;


TEST_CASE("brightness", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });

    auto color = camera.getColorBuffer();
    color[0] = { 87, 56,  38 };
    color[1] = { 71, 83,  26 };
    color[2] = { 50, 29, 105 };

    Brightness(56).run(camera);

    REQUIRE(color[0].equals(Vec3f{ 143, 112, 94 }));
    REQUIRE(color[1].equals(Vec3f{ 127, 139, 82 }));
    REQUIRE(color[2].equals(Vec3f{ 106, 85, 161 }));
}


TEST_CASE("color filter", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });

    auto color = camera.getColorBuffer();
    color[0] = {  63, 82,  71 };
    color[1] = {  63, 78, 214 };
    color[2] = { 205, 46,  92 };

    ColorFilter({ 84, 45, 69 }).run(camera);

    REQUIRE(color[0].equals(Vec3f{ 20.7529f, 14.4706f, 19.2118f }));
    REQUIRE(color[1].equals(Vec3f{ 20.7529f, 13.7647f, 57.9059f }));
    REQUIRE(color[2].equals(Vec3f{ 67.5294f, 8.11765f, 24.8941f }));
}


TEST_CASE("contrast", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });

    auto color = camera.getColorBuffer();
    color[0] = { 91, 255, 64 };
    color[1] = { 78,  93, 28 };
    color[2] = { 74,  76, 50 };

    Contrast(1.9f).run(camera);

    REQUIRE(color[0].equals(Vec3f{ 58.15f, 255.0f, 6.85f }));
    REQUIRE(color[1].equals(Vec3f{ 33.45f, 61.95f, 0.0f }));
    REQUIRE(color[2].equals(Vec3f{ 25.85f, 29.65f, 0.0f }));
}


TEST_CASE("exposure", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });

    auto color = camera.getColorBuffer();
    color[0] = { 193,  29, 46 };
    color[1] = {  72,  32, 89 };
    color[2] = {  35, 107, 63 };

    Exposure(3).run(camera);

    REQUIRE(color[0].equals(Vec3f{ 255,  87, 138 }));
    REQUIRE(color[1].equals(Vec3f{ 216,  96, 255 }));
    REQUIRE(color[2].equals(Vec3f{ 105, 255, 189 }));
}


TEST_CASE("saturation", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });

    auto color = camera.getColorBuffer();
    color[0] = { 203, 172, 89 };
    color[1] = {  90, 125, 39 };
    color[2] = { 107,  84, 59 };

    Saturation(2).run(camera);

    REQUIRE(color[0].equals(Vec3f{ 233.402f, 171.402f, 5.40199f }));
    REQUIRE(color[1].equals(Vec3f{ 68.6502f, 138.65f, 0.0f }));
    REQUIRE(color[2].equals(Vec3f{ 126.915f, 80.9152f, 30.9152f }));
}


TEST_CASE("tone mapping", "[effects]") {
    Camera camera;
    camera.setRes({ 3, 1 });
    auto color = camera.getColorBuffer();
    color[0] = { 100,   25,  25 };
    color[1] = { 100, 2550, 100 };
    color[2] = { 300,  255, 255 };

    ToneMapping().run(camera);

    REQUIRE(color[0].equals(Vec3f{ 86.4269f, 21.6067f, 21.6067f }));
    REQUIRE(color[1].equals(Vec3f{ 13.7671f, 351.061f, 13.7671f }));
    REQUIRE(color[2].equals(Vec3f{ 150.133f, 127.613f, 127.613f }));
}


TEST_CASE("fog", "[effects]") {
    Camera camera;
    camera.setRes({ 5, 1 });
    auto color = camera.getColorBuffer();
    auto depth = camera.getDepthBuffer();
    for (size_t i = 0; i < 5; ++i) {
        color[i] = vec3f_255;
        depth[i] = (float)(i + 1) * 0.5f;
    }

    Fog(vec3f_0, 1, 2).run(camera);

    REQUIRE(color[0].equals(vec3f_255));
    REQUIRE(color[1].equals(vec3f_255));
    REQUIRE(color[2].equals(Vec3f{ 127.5f, 127.5f, 127.5f }));
    REQUIRE(color[3].equals(vec3f_0));
    REQUIRE(color[4].equals(vec3f_0));
}
