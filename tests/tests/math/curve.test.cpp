#include <array>

#include <ramiel/test.h>
#include <ramiel/util.h>
#include <ramiel/math.h>
using namespace ramiel;
using namespace ramiel::test;

RAMIEL_TEST_ADD(curve) {
    std::array<Vec2f, 4> crv = {
        Vec2f{ -1.0f, -1.0f },
        Vec2f{  0.0f,  1.0f },
        Vec2f{  0.0f, -1.0f },
        Vec2f{  1.0f,  1.0f }
    };
    float t = 0.8f;

    Vec2f expected = { 0.504f, 0.216f };
    Vec2f actual = crvSolve(crv.data(), crv.size() - 1, t);
    RAMIEL_TEST_ASSERT(equal(expected, actual));
    std::cout << actual << '\n';
}

