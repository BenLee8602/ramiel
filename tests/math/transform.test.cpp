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
