#include <ramiel/test.h>
#include <ramiel/graphics.h>
using namespace ramiel;


RAMIEL_TEST_ADD(CameraBufferSize) {
    setRes({ 10, 10 });
    RAMIEL_TEST_ASSERT((getRes() == Vec2u{ 10, 10 }));
    RAMIEL_TEST_ASSERT(getBufferSize() == 100);
}


RAMIEL_TEST_ADD(CameraGetCameraCoord) {
    Vec4f in;
    Vec4f expected;

    in = { -4.8f, 2.3f, 7.6f, 1.0f };
    expected = { -4.8f, 2.3f, 7.6f, 1.0f };
    setRot({ 0.0f, 0.0f, 0.0f });
    setPos({ 0.0f, 0.0f, 0.0f });
    RAMIEL_TEST_ASSERT(equal(getCameraCoord(in), expected));

    in = { 9.9f, -5.3f, -9.7f, 1.0f };
    expected = { 10.1742f, 6.55266f, 8.58715f, 1.0f };
    setRot({ -5.4f, -1.3f, -4.5f });
    setPos({ 0.0f, 0.0f, 0.0f });
    RAMIEL_TEST_ASSERT(equal(getCameraCoord(in), expected));

    in = { -2.1f, 6.8f, 0.5f, 1.0f };
    expected = { -7.9f, 15.3f, 2.1f, 1.0f };
    setRot({ 0.0f, 0.0f, 0.0f });
    setPos({ 5.8f, -8.5f, -1.6f });
    RAMIEL_TEST_ASSERT(equal(getCameraCoord(in), expected));

    in = { 2.5f, -6.6f, -3.1f, 1.0f };
    expected = { -2.05688f, 5.63915f, 12.0108f, 1.0f };
    setRot({ -9.3f, 5.6f, 3.8f });
    setPos({ -8.7f, -6.9f, 4.3f });
    RAMIEL_TEST_ASSERT(equal(getCameraCoord(in), expected));
}
