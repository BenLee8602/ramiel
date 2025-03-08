#include <ramiel/test.h>
#include <ramiel/data.h>
#include <ramiel/file.h>
using namespace ramiel;


RAMIEL_TEST_ADD(TextureSize) {
    Texture texture(std::string(ramiel_TEST_DATA_DIR) + "/colortest.png");
    Vec2 expected = { 640, 480 };
    RAMIEL_TEST_ASSERT(texture.getSize() == expected);
}

RAMIEL_TEST_ADD(TextureGetColor) {
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

    RAMIEL_TEST_ASSERT(equal(texture.get({  1.0f / 16.0f,  1.0f / 16.0f }), white));
    RAMIEL_TEST_ASSERT(equal(texture.get({  3.0f / 16.0f,  3.0f / 16.0f }), yellow));
    RAMIEL_TEST_ASSERT(equal(texture.get({  5.0f / 16.0f,  5.0f / 16.0f }), cyan));
    RAMIEL_TEST_ASSERT(equal(texture.get({  7.0f / 16.0f,  7.0f / 16.0f }), green));
    RAMIEL_TEST_ASSERT(equal(texture.get({  9.0f / 16.0f,  9.0f / 16.0f }), magenta));
    RAMIEL_TEST_ASSERT(equal(texture.get({ 11.0f / 16.0f, 11.0f / 16.0f }), red));
    RAMIEL_TEST_ASSERT(equal(texture.get({ 13.0f / 16.0f, 13.0f / 16.0f }), blue));
    RAMIEL_TEST_ASSERT(equal(texture.get({ 15.0f / 16.0f, 15.0f / 16.0f }), black));
}

RAMIEL_TEST_ADD(TextureGetNormalVector) {
    Texture texture(std::string(ramiel_TEST_DATA_DIR) + "/colortest.png", normalMap);

    const float n = 0.992157f;
    const Vec3f expected1 = { -1.0f, -1.0f, -1.0f };
    const Vec3f expected2 = { -1.0f,     n, -1.0f };
    const Vec3f expected3 = { -1.0f,     n,     n };
    const Vec3f expected4 = {     n,     n,     n };
    RAMIEL_TEST_ASSERT(equal(texture.get({ 15.0f / 16.0f, 0.5f }), expected1));
    RAMIEL_TEST_ASSERT(equal(texture.get({  7.0f / 16.0f, 0.5f }), expected2));
    RAMIEL_TEST_ASSERT(equal(texture.get({  5.0f / 16.0f, 0.5f }), expected3));
    RAMIEL_TEST_ASSERT(equal(texture.get({  1.0f / 16.0f, 0.5f }), expected4));
}
