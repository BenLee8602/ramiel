#include "catch2/catch2.hpp"
#include <vec.h>
using namespace ramiel;


TEST_CASE("boolean conversion", "[vec][bool]") {
    Vec3 vec_false = { 0, 0, 0 };
    REQUIRE_FALSE(vec_false);

    Vec3 vec_true = { 0, 0, 1 };
    REQUIRE(vec_true);
}

TEST_CASE("comparison operator", "[vec][bool]") {
    Vec3 v1 = { 10, -1, 3 };
    Vec3 v2 = { 7, 5, -8 };
    Vec3 v3 = { 10, -1, 3 };
    REQUIRE_FALSE(v1 == v2);
    REQUIRE_FALSE(v2 == v3);
    REQUIRE(v1 == v3);
}

TEST_CASE("floating point vec comparison", "[vec][bool]") {
    Vec3f v1 = { -5.8f, 3.2f, -3.9f };
    Vec3f v2 = { -9.9f, 8.7f, 2.3f };
    Vec3f v3 = { -5.8f, 3.2f, -3.899999f };
    REQUIRE_FALSE(v1.equals(v2));
    REQUIRE_FALSE(v2.equals(v3));
    REQUIRE(v1.equals(v3));
}

TEST_CASE("vector assignment", "[vec][vector][assignment]") {
    Vec3 original = { 1, -4, 2 };
    Vec3 copy = original;
    REQUIRE(copy == original);
}

TEST_CASE("scalar assignment", "[vec][scalar][assignment]") {
    Vec3 v;
    v = 5;
    Vec3 expected = { 5, 5, 5 };
    REQUIRE(v == expected);
}

TEST_CASE("operator+=(scalar)", "[vec][scalar][assignment]") {
    Vec3 left = { -4, -9, -7 };
    int right = -9;
    Vec3 expected = { -13, -18, -16 };
    left += right;
    REQUIRE(left == expected);
}

TEST_CASE("operator-=(scalar)", "[vec][scalar][assignment]") {
    Vec3 left = { 9, 6, -3 };
    int right = 5;
    Vec3 expected = { 4, 1, -8 };
    left -= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator*=(scalar)", "[vec][scalar][assignment]") {
    Vec3 left = { 0, -5, -1 };
    int right = 6;
    Vec3 expected = { 0, -30, -6 };
    left *= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator/=(scalar)", "[vec][scalar][assignment]") {
    Vec3 left = { -1, 8, 3 };
    int right = -3;
    Vec3 expected = { 0, -2, -1 };
    left /= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator+=(vec)", "[vec][vector][assignment]") {
    Vec3 left = { 7, 9, 2 };
    Vec3 right = { -5, 8, 0 };
    Vec3 expected = { 2, 17, 2 };
    left += right;
    REQUIRE(left == expected);
}

TEST_CASE("operator-=(vec)", "[vec][vector][assignment]") {
    Vec3 left = { 7, -2, 9 };
    Vec3 right = { 6, -5, -2 };
    Vec3 expected = { 1, 3, 11 };
    left -= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator*=(vec)", "[vec][vector][assignment]") {
    Vec3 left = { -6, -9, -8 };
    Vec3 right = { -7, 3, 7 };
    Vec3 expected = { 42, -27, -56 };
    left *= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator/=(vec)", "[vec][vector][assignment]") {
    Vec3 left = { 5, 5, 7 };
    Vec3 right = { 5, 2, 5 };
    Vec3 expected = { 1, 2, 1 };
    left /= right;
    REQUIRE(left == expected);
}

TEST_CASE("operator+(scalar)", "[vec][scalar]") {
    Vec3 left = { 0, 8, -5 };
    int right = -4;
    Vec3 expected = { -4, 4, -9 };
    Vec3 res = left + right;
    REQUIRE(res == expected);
}

TEST_CASE("operator-(scalar)", "[vec][scalar]") {
    Vec3 left = { -2, -3, -1 };
    int right = 6;
    Vec3 expected = { -8, -9, -7 };
    Vec3 res = left - right;
    REQUIRE(res == expected);
}

TEST_CASE("operator*(scalar)", "[vec][scalar]") {
    Vec3 left = { 8, -8, -9 };
    int right = 7;
    Vec3 expected = { 56, -56, -63 };
    Vec3 res = left * right;
    REQUIRE(res == expected);
}

TEST_CASE("operator/(scalar)", "[vec][scalar]") {
    Vec3 left = { -2, -6, -5 };
    int right = 9;
    Vec3 expected = { 0, 0, 0 };
    Vec3 res = left / right;
    REQUIRE(res == expected);
}

TEST_CASE("operator+(vec)", "[vec][vector]") {
    Vec3 left = { -5, -6, -8 };
    Vec3 right = { 2, 1, -1 };
    Vec3 expected = { -3, -5, -9 };
    Vec3 res = left + right;
    REQUIRE(res == expected);
}

TEST_CASE("operator-(vec)", "[vec][vector]") {
    Vec3 left = { 3, 7, 8 };
    Vec3 right = { -7, -5, -3 };
    Vec3 expected = { 10, 12, 11 };
    Vec3 res = left - right;
    REQUIRE(res == expected);
}

TEST_CASE("operator*(vec)", "[vec][vector]") {
    Vec3 left = { 8, -7, 4 };
    Vec3 right = { 0, 4, -7 };
    Vec3 expected = { 0, -28, -28 };
    Vec3 res = left * right;
    REQUIRE(res == expected);
}

TEST_CASE("operator/(vec)", "[vec][vector]") {
    Vec3 left = { -2, 2, 8 };
    Vec3 right = { -6, 2, 6 };
    Vec3 expected = { 0, 1, 1 };
    Vec3 res = left / right;
    REQUIRE(res == expected);
}

TEST_CASE("operator-()", "[vec]") {
    Vec3f v = { 0.2f, 5.5f, -5.7f };
    Vec3f res = -v;
    Vec3f expected = { -0.2f, -5.5f, 5.7f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("magnitude", "[vec]") {
    Vec3f v = { -8.9f, -7.7f, -5.4f };
    float res = getMagnitude(v);
    Approx expected = Approx(12.94836f).epsilon(0.0001f);
    REQUIRE(res == expected);
}

TEST_CASE("normalize", "[vec]") {
    Vec3f v = { -5.2f, 3.3f, 8.5f };
    Vec3f res = getNormalized(v);
    Vec3f expected = { -0.495395f, 0.314386f, 0.809781f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("normalize, magnitude given", "[vec]") {
    Vec3f v = { -3.2f, -1.4f, 8.8f };
    Vec3f res = getNormalized(v, 9.46784f);
    Vec3f expected = { -0.33799f, -0.14786f, 0.92946f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("dot product", "[vec]") {
    Vec3f v1 = { -9.2f, 6.9f, 9.9f };
    Vec3f v2 = { -8.2f, -3.5f, 4.6f };
    float res = dotProduct(v1, v2);
    Approx expected = Approx(96.83f).epsilon(0.0001f);
    REQUIRE(res == expected);
}

TEST_CASE("cross product", "[vec]") {
    Vec3f v1 = { 9.1f, -4.9f, 0.4f };
    Vec3f v2 = { -5.0f, -8.2f, -7.3f };
    Vec3f res = crossProduct(v1, v2);
    Vec3f expected = { 39.05f, 64.43f, -99.12f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("color clamp min", "[vec][color]") {
    Vec3f color = { 255.0f, 300.0f, 200.0f };
    c_min(color);
    Vec3f expected = { 255.0f, 255.0f, 200.0f };
    REQUIRE(color == expected);
}

TEST_CASE("color clamp max", "[vec][color]") {
    Vec3f color = { 255.0f, -5.0f, 100.0f };
    c_max(color);
    Vec3f expected = { 255.0f, 0.0f, 100.0f };
    REQUIRE(color == expected);
}

TEST_CASE("color clamp", "[vec][color]") {
    Vec3f color = { -999.0f, 128.0f, 999.0f };
    c_clamp(color);
    Vec3f expected = { 0.0f, 128.0f, 255.0f };
    REQUIRE(color == expected);
}

TEST_CASE("rgb to dec", "[vec][color]") {
    Vec3f color = { 255.0f, 255.0f, 255.0f };
    int res = rgbToDec(color);
    int expected = 0xffffff;
    REQUIRE(res == expected);
}

TEST_CASE("ldr", "[vec][tonemapping]") {
    std::vector<Vec3f> color = { { 300.0f, 0.0f, 100.0f } };
    ldr(color);
    Vec3f expected = { 255.0f, 27.987804f, 117.012195f };
    REQUIRE(color[0] == expected);
}
