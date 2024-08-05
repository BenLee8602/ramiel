#include <ramiel/test.h>
#include <ramiel/math.h>
using namespace ramiel;


RAMIEL_TEST_ADD(TransformTranslate) {
    Mat4x4 expected = {
        Vec4{ 1, 0, 0, 3 },
        Vec4{ 0, 1, 0, 6 },
        Vec4{ 0, 0, 1, 9 },
        Vec4{ 0, 0, 0, 1 }
    };
    Mat4x4 actual = translate(Vec3{ 3, 6, 9 });
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(TransformRotate2D) {
    Mat3x3f expected = {
        Vec3f{  0.540302f, -0.841471f,  0.0f  },
        Vec3f{  0.841471f,  0.540302f,  0.0f  },
        Vec3f{     0.0f,       0.0f,    1.0f  }
    };
    Mat3x3f actual = rotate(1.0f);
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(TransformRotate3D) {
    Mat4x4f expected = {
        Vec4f{  0.095105f,  0.646688f, -0.756802f,  0.0f  },
        Vec4f{ -0.311591f,  0.741385f,  0.594356f,  0.0f  },
        Vec4f{  0.945445f,  0.179287f,  0.272012f,  0.0f  },
        Vec4f{     0.0f,       0.0f,       0.0f,    1.0f  }
    };
    Mat4x4f actual = rotate(Vec3f{ 2.0f, 4.0f, 8.0f });
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}


RAMIEL_TEST_ADD(TransformScale) {
    Mat4x4f expected = {
        Vec4f{ 0.5f, 0.0f, 0.0f, 0.0f },
        Vec4f{ 0.0f, 1.0f, 0.0f, 0.0f },
        Vec4f{ 0.0f, 0.0f, 2.0f, 0.0f },
        Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f }
    };
    Mat4x4f actual = scale(Vec3f{ 0.5f, 1.0f, 2.0f });
    RAMIEL_TEST_ASSERT(equal(expected, actual));
}
