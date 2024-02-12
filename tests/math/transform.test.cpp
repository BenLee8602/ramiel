#include <catch2/catch2.hpp>
#include <ramiel/math.h>
using namespace ramiel;

TEST_CASE("transform vector", "[transform]") {
    Transform transform;
    Vec3f in;
    Vec3f expected;

    transform.pos = {  4.9f, -1.3f, 8.4f };
    transform.rot = { -3.5f, -7.6f, 1.2f };
    transform.scale = 2.5f;
    in = { 1.4f, -9.0f, 3.4f };
    expected = { 2.26049, -5.01989, -15.4735 };
    REQUIRE(equal(transform(in), expected));

    transform.pos = { -3.0f, -4.8f, -7.2f };
    transform.rot = { -4.0f,  7.2f,  8.2f };
    transform.scale = 3.1f;
    in = { 7.0f, -8.5f, 9.4f };
    expected = { 30.7302, -26.9324, 12.4667 };
    REQUIRE(equal(transform(in), expected));
}


TEST_CASE("translation matrix", "[transform]") {
    Mat4x4 expected = {
        Vec4{ 1, 0, 0, 3 },
        Vec4{ 0, 1, 0, 6 },
        Vec4{ 0, 0, 1, 9 },
        Vec4{ 0, 0, 0, 1 }
    };
    Mat4x4 actual = translate(Vec3{ 3, 6, 9 });
    REQUIRE(expected == actual);
}


TEST_CASE("2d rotation matrix", "[transform]") {
    Mat3x3f expected = {
        Vec3f{  0.540302f, -0.841471f,  0.0f  },
        Vec3f{  0.841471f,  0.540302f,  0.0f  },
        Vec3f{     0.0f,       0.0f,    1.0f  }
    };
    Mat3x3f actual = rotate(1.0f);
    REQUIRE(equal(expected, actual));
}


TEST_CASE("3d rotation matrix", "[transform]") {
    Mat4x4f expected = {
        Vec4f{  0.095105f,  0.646688f, -0.756802f,  0.0f  },
        Vec4f{ -0.311591f,  0.741385f,  0.594356f,  0.0f  },
        Vec4f{  0.945445f,  0.179287f,  0.272012f,  0.0f  },
        Vec4f{     0.0f,       0.0f,       0.0f,    1.0f  }
    };
    Mat4x4f actual = rotate(Vec3f{ 2.0f, 4.0f, 8.0f });
    REQUIRE(equal(expected, actual));
}


TEST_CASE("scaling matrix", "[transform]") {
    Mat4x4f expected = {
        Vec4f{ 0.5f, 0.0f, 0.0f, 0.0f },
        Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
        Vec4f{ 0.0f, 0.0f, 2.0f, 0.0f },
        Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f }
    };
    Mat4x4f actual = scale(Vec3f{ 0.5f, 1.0f, 2.0f });
    REQUIRE(equal(expected, actual));
}
