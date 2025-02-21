#include <ramiel/test.h>
#include <ramiel/graphics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(LightDirectional) {
    DirectionalLight light({ 42, 174, 183 }, 2.5f, { -2.2f, 2.5f, 6.0f });
    Vec3f pos = { 6.7f, 5.8f, 2.1f };
    Vec3f normal = normalize(Vec3f{ 9.2f, -4.3f, -1.5f });
    Vec3f res = light.getLight(pos, normal, 10, 0.1f);
    Vec3f expected = { 59.607f, 246.943f, 259.716f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}

RAMIEL_TEST_ADD(LightPoint) {
    PointLight light({ 198, 133, 66 }, 7.8f, { -2.3f, -4.4f, -1.6f }, 0.2f);
    Vec3f pos = { 0.1f, -4.4f, -2.7f };
    Vec3f normal = normalize(Vec3f{ -5.2f, 1.6f, 3.3f });
    Vec3f res = light.getLight(pos, normal, 64, 0.8f);
    Vec3f expected = { 618.644f, 415.554f, 206.215f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}

RAMIEL_TEST_ADD(LightSpot) {
    SpotLight light({ 126, 215, 181 }, 2.1f, Vec3f(), { 0.4f, -0.3f, 1.0f }, 0.1f, 1.0472f, 50.0f);
    Vec3f pos = { 1.5f, -1.6f, 4.1f };
    Vec3f normal = normalize(Vec3f{ 0.2f, 0.4f, -2.0f });
    Vec3f res = light.getLight(pos, normal, 36, 0.3f);
    Vec3f expected = { 62.7161f, 107.016f, 90.0922f };
    RAMIEL_TEST_ASSERT(equal(res, expected, 0.001f));
}
