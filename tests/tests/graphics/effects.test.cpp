#include <ramiel/test.h>
#include <vector>
#include <ramiel/graphics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(EffectBrightness) {
    Brightness effect{ 56 };

    Vec3f in = { 87, 56, 38 };
    Vec3f expected = { 143, 112, 94 };
    Vec3f actual = effect(in);

    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(EffectContrast) {
    Contrast effect{ 1.9f };

    Vec3f in = { 91, 255, 64 };
    Vec3f expected = { 58.15f, 255.0f, 6.85f };
    Vec3f actual = effect(in);
    
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(EffectExposure) {
    Exposure effect{ 3 };

    Vec3f in = { 193, 29, 46 };
    Vec3f expected = { 255, 87, 138 };
    Vec3f actual = effect(in);
    
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(EffectSaturation) {
    Saturation effect{ 2 };

    Vec3f in = { 203, 172, 89 };
    Vec3f expected = { 233.402f, 171.402f, 5.40199f };
    Vec3f actual = effect(in);
    
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(EffectFog) {
    Fog effect({ 255, 128, 0 }, 1.0f, 2.0f);

    RAMIEL_TEST_ASSERT(equal(effect({ 128, 128, 128 }, 0.75f), Vec3f{ 128, 128, 128 }));
    RAMIEL_TEST_ASSERT(equal(effect({ 128, 128, 128 }, 1.25f), Vec3f{ 159.75f, 128.0f, 96.0f }));
    RAMIEL_TEST_ASSERT(equal(effect({ 128, 128, 128 }, 1.75f), Vec3f{ 223.25f, 128.0f, 32.0f }));
    RAMIEL_TEST_ASSERT(equal(effect({ 128, 128, 128 }, 2.25f), Vec3f{ 255, 128, 0 }));
}
