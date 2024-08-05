#include <ramiel/test.h>
#include <ramiel/graphics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(LightDirectional) {
    DirectionalLight light({ 65, 32, 98 }, 5.1f, { -0.5f, -1.9f, 0.8f });
    Vec3f pos = { -6.0f, -9.1f, -8.2f };
    Vec3f normal = normalize(Vec3f{ 5.6f, 9.5f, -9.1f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 307.458f, 151.364f, 463.552f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}

RAMIEL_TEST_ADD(LightPoint) {
    PointLight light({ 78, 208, 29 }, 4.7f, { -9.1f, -6.9f, 5.4f }, 0.2f);
    Vec3f pos = { -4.3f, -2.5f, 8.9f };
    Vec3f normal = normalize(Vec3f{ -4.7f, -1.4f, 7.5f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 1.14577f, 3.05538f, 0.425991f };
    RAMIEL_TEST_ASSERT(equal(res, expected));
}

RAMIEL_TEST_ADD(LightSpot) {
    SpotLight light({ 175, 139, 43 }, 7.1f, { 4.6f, -0.3f, 2.7f }, { 5.9f, -0.7f, 7.1f }, 0.1f, 1.74777f, 81.2f);
    Vec3f pos = { 10.5f, -1.0f, 9.8f };
    Vec3f normal = normalize(Vec3f{ -2.5f, 0.7f, -8.1f });
    Vec3f res = light.getLight(pos, normal);
    Vec3f expected = { 119.931f, 95.2598f, 29.4689f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}


RAMIEL_TEST_ADD(LightDirectionalSpecular) {
    DirectionalLight light({ 42, 174, 183 }, 2.5f, { -2.2f, 2.5f, 6.0f });
    Vec3f pos = { 6.7f, 5.8f, 2.1f };
    Vec3f normal = normalize(Vec3f{ 9.2f, -4.3f, -1.5f });
    Vec3f res = light.getLight(pos, normal, Vec3f(), 10, 0.1f);
    Vec3f expected = { 59.607f, 246.943f, 259.716f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}

RAMIEL_TEST_ADD(LightPointSpecular) {
    PointLight light({ 198, 133, 66 }, 7.8f, { -2.3f, -4.4f, -1.6f }, 0.2f);
    Vec3f pos = { 0.1f, -4.4f, -2.7f };
    Vec3f normal = normalize(Vec3f{ -5.2f, 1.6f, 3.3f });
    Vec3f res = light.getLight(pos, normal, Vec3f(), 64, 0.8f);
    Vec3f expected = { 618.644f, 415.554f, 206.215f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}

RAMIEL_TEST_ADD(LightSpotSpecular) {
    SpotLight light({ 126, 215, 181 }, 2.1f, Vec3f(), { 0.4f, -0.3f, 1.0f }, 0.1f, 1.0472f, 50.0f);
    Vec3f pos = { 1.5f, -1.6f, 4.1f };
    Vec3f normal = normalize(Vec3f{ 0.2f, 0.4f, -2.0f });
    Vec3f res = light.getLight(pos, normal, Vec3f(), 36, 0.3f);
    Vec3f expected = { 62.7161f, 107.016f, 90.0922f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}
