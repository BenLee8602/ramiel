#include <ramiel/test.h>
#include <vector>
#include <ramiel/graphics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(EffectBrightness) {
    setRes({ 3, 1 });

    auto color = getColorBuffer();
    color[0] = { 87, 56,  38 };
    color[1] = { 71, 83,  26 };
    color[2] = { 50, 29, 105 };

    Brightness(56).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 143, 112, 94 }));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 127, 139, 82 }));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 106, 85, 161 }));
}


RAMIEL_TEST_ADD(EffectColorFilter) {
    setRes({ 3, 1 });

    auto color = getColorBuffer();
    color[0] = {  63, 82,  71 };
    color[1] = {  63, 78, 214 };
    color[2] = { 205, 46,  92 };

    ColorFilter({ 84, 45, 69 }).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 20.7529f, 14.4706f, 19.2118f }));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 20.7529f, 13.7647f, 57.9059f }));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 67.5294f, 8.11765f, 24.8941f }));
}


RAMIEL_TEST_ADD(EffectContrast) {
    setRes({ 3, 1 });

    auto color = getColorBuffer();
    color[0] = { 91, 255, 64 };
    color[1] = { 78,  93, 28 };
    color[2] = { 74,  76, 50 };

    Contrast(1.9f).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 58.15f, 255.0f, 6.85f }));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 33.45f, 61.95f, 0.0f }));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 25.85f, 29.65f, 0.0f }));
}


RAMIEL_TEST_ADD(EffectExposure) {
    setRes({ 3, 1 });

    auto color = getColorBuffer();
    color[0] = { 193,  29, 46 };
    color[1] = {  72,  32, 89 };
    color[2] = {  35, 107, 63 };

    Exposure(3).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 255,  87, 138 }));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 216,  96, 255 }));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 105, 255, 189 }));
}


RAMIEL_TEST_ADD(EffectSaturation) {
    setRes({ 3, 1 });

    auto color = getColorBuffer();
    color[0] = { 203, 172, 89 };
    color[1] = {  90, 125, 39 };
    color[2] = { 107,  84, 59 };

    Saturation(2).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 233.402f, 171.402f, 5.40199f }, 0.001f));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 68.6502f, 138.65f, 0.0f }, 0.001f));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 126.915f, 80.9152f, 30.9152f }, 0.001f));
}


RAMIEL_TEST_ADD(EffectToneMapping) {
    setRes({ 3, 1 });
    auto color = getColorBuffer();
    color[0] = { 100,   25,  25 };
    color[1] = { 100, 2550, 100 };
    color[2] = { 300,  255, 255 };

    ToneMapping().run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 86.4269f, 21.6067f, 21.6067f }, 0.001f));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 13.7671f, 351.061f, 13.7671f }, 0.001f));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 150.133f, 127.613f, 127.613f }, 0.001f));
}


RAMIEL_TEST_ADD(EffectFog) {
    setRes({ 5, 1 });
    auto color = getColorBuffer();
    auto depth = getDepthBuffer();
    for (size_t i = 0; i < 5; ++i) {
        color[i] = Vec3f{ 255, 255, 255 };
        depth[i] = (float)(i + 1) * 0.5f;
    }

    Fog(Vec3f(), 1, 2).run();

    RAMIEL_TEST_ASSERT(equal(color[0], Vec3f{ 255, 255, 255 }));
    RAMIEL_TEST_ASSERT(equal(color[1], Vec3f{ 255, 255, 255 }));
    RAMIEL_TEST_ASSERT(equal(color[2], Vec3f{ 127.5f, 127.5f, 127.5f }));
    RAMIEL_TEST_ASSERT(equal(color[3], Vec3f()));
    RAMIEL_TEST_ASSERT(equal(color[4], Vec3f()));
}
