#include "catch2/catch2.hpp"

#include <light.h>
using namespace ramiel;


TEST_CASE("get directional light", "[light]") {
    DirectionalLight light({ 65, 32, 98 }, 5.1f, { -0.5f, -1.9f, 0.8f });
    Vec3f pos = { -6.0f, -9.1f, -8.2f };
    Vec3f normal = getNormalized({ 5.6f, 9.5f, -9.1f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 307.458f, 151.364f, 463.552f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get point light", "[light]") {
    PointLight light({ 78, 208, 29 }, 4.7f, { -9.1f, -6.9f, 5.4f }, 0.2f);
    Vec3f pos = { -4.3f, -2.5f, 8.9f };
    Vec3f normal = getNormalized({ -4.7f, -1.4f, 7.5f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 1.14577f, 3.05538f, 0.425991f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get spot light", "[light]") {
    SpotLight light({ 175, 139, 43 }, 7.1f, { 4.6f, -0.3f, 2.7f }, { 5.9f, -0.7f, 7.1f }, 0.1f, 50.7f, 81.2f);
    Vec3f pos = { 10.5f, -1.0f, 9.8f };
    Vec3f normal = getNormalized({ -2.5f, 0.7f, -8.1f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 119.931f, 95.2598f, 29.4689f };
    REQUIRE(res.equals(expected));
}


TEST_CASE("get directional light specular", "[light]") {
    DirectionalLight light({ 42, 174, 183 }, 2.5f, { -2.2f, 2.5f, 6.0f });
    Vec3f pos = { 6.7f, 5.8f, 2.1f };
    Vec3f normal = getNormalized({ 9.2f, -4.3f, -1.5f });
    Vec3f res = light.getLight(pos, normal, vec3f_0, 10, 0.1f);
    Vec3f expected = { 59.607f, 246.943f, 259.716f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get point light specular", "[light]") {
    PointLight light({ 198, 133, 66 }, 7.8f, { -2.3f, -4.4f, -1.6f }, 0.2f);
    Vec3f pos = { 0.1f, -4.4f, -2.7f };
    Vec3f normal = getNormalized({ -5.2f, 1.6f, 3.3f });
    Vec3f res = light.getLight(pos, normal, vec3f_0, 64, 0.8f);
    Vec3f expected = { 618.644f, 415.554f, 206.215f };
    REQUIRE(res.equals(expected));
}

TEST_CASE("get spot light specular", "[light]") {
    SpotLight light({ 126, 215, 181 }, 2.1f, vec3f_0, { 0.4f, -0.3f, 1.0f }, 0.1f, 30.0f, 50.0f);
    Vec3f pos = { 1.5f, -1.6f, 4.1f };
    Vec3f normal = getNormalized({ 0.2f, 0.4f, -2.0f });
    Vec3f res = light.getLight(pos, normal, vec3f_0, 36, 0.3f);
    Vec3f expected = { 62.7161f, 107.016f, 90.0922f };
    REQUIRE(res.equals(expected));
}
