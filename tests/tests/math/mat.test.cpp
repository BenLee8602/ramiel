#include <ramiel/test.h>
#include <ramiel/math.h>
using namespace ramiel;


RAMIEL_TEST_ADD(MatrixVectorMultiplication) {
    Vec3 v = { 1, 2, 3 };
    Mat2x3 m = {
        Vec3{ 4, 5, 6 },
        Vec3{ 7, 8, 9 }
    };
    Vec2 expected = { 32, 50 };
    Vec2 actual = matvec(m, v);
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(MatrixMatrixMultiplicaion) {
    Mat3x2 m1 = {
        Vec2{ 1, 2 },
        Vec2{ 3, 4 },
        Vec2{ 5, 6 }
    };
    Mat2x4 m2 = {
        Vec4{  7,  8,  9, 10 },
        Vec4{ 11, 12, 13, 14 }
    };
    Mat3x4 expected = {
        Vec4{  29,  32,  35,  38 },
        Vec4{  65,  72,  79,  86 },
        Vec4{ 101, 112, 123, 134 }
    };
    Mat3x4 actual = matmat(m2, m1);
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(MatrixIdentity) {
    Mat3x3 expected = {
        Vec3{ 1, 0, 0 },
        Vec3{ 0, 1, 0 },
        Vec3{ 0, 0, 1 }
    };
    Mat3x3 actual = id<int, 3>();
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(MatrixTranspose) {
    Mat2x3 in = {
        Vec3{ 1, 2, 3 },
        Vec3{ 4, 5, 6 }
    };
    Mat3x2 expected = {
        Vec2{ 1, 4 },
        Vec2{ 2, 5 },
        Vec2{ 3, 6 }
    };
    Mat3x2 actual = tp(in);
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(MatrixSubmatrix) {
    Mat4x4 in = {
        Vec4{  1,  2,  3,  4 },
        Vec4{  5,  6,  7,  8 },
        Vec4{  9, 10, 11, 12 },
        Vec4{ 13, 14, 15, 16 }
    };
    Mat3x3 expected = {
        Vec3{  1,  2,  4 },
        Vec3{  9, 10, 12 },
        Vec3{ 13, 14, 16 }
    };
    Mat3x3 actual = sub(in, 1, 2);
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(MatrixDeterminant) {
    Vec<Vec<int, 1>, 1> in1 = {
        Vec<int, 1>{ 1 }
    };
    int expected1 = 1;
    int actual1 = det(in1);
    RAMIEL_TEST_ASSERT(expected1 == actual1);

    Mat2x2 in2 = {
        Vec2{ 1, 2 },
        Vec2{ 3, 4 }
    };
    int expected2 = -2;
    int actual2 = det(in2);
    RAMIEL_TEST_ASSERT(expected2 == actual2);

    Mat3x3 in3 = {
        Vec3{ 1, 2, 3 },
        Vec3{ 4, 5, 6 },
        Vec3{ 7, 8, 9 }
    };
    int expected3 = 0;
    int actual3 = det(in3);
    RAMIEL_TEST_ASSERT(expected3 == actual3);

    Mat4x4 in4 = {
        Vec4{ 1, 3, 5, 9 },
        Vec4{ 1, 3, 1, 7 },
        Vec4{ 4, 3, 9, 7 },
        Vec4{ 5, 2, 0, 9 }
    };
    int expected4 = -376;
    int actual4 = det(in4);
    RAMIEL_TEST_ASSERT(expected4 == actual4);
}


RAMIEL_TEST_ADD(MatrixInversion) {
    Mat4x4 in1 = {
        Vec4{ 1, 0, 0, 2 },
        Vec4{ 0, 1, 0, 4 },
        Vec4{ 0, 0, 1, 8 },
        Vec4{ 0, 0, 0, 1 }
    };
    Mat4x4 expected1 = {
        Vec4{ 1, 0, 0, -2 },
        Vec4{ 0, 1, 0, -4 },
        Vec4{ 0, 0, 1, -8 },
        Vec4{ 0, 0, 0,  1 }
    };
    Mat4x4 actual1 = inv(in1);
    RAMIEL_TEST_ASSERT(expected1 == actual1);

    Mat2x2 notInvertable = {
        Vec2{ 0, 0 },
        Vec2{ 1, 0 }
    };
    RAMIEL_TEST_ASSERT(inv(notInvertable) == Mat2x2());
}
