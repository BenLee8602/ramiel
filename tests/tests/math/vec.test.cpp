#include <ramiel/test.h>
#include <ramiel/math.h>
using namespace ramiel;


RAMIEL_TEST_ADD(VecVectorConvertBool) {
    Vec3 vec_false = { 0, 0, 0 };
    RAMIEL_TEST_ASSERT(!vec_false);

    Vec3 vec_true = { 0, 0, 1 };
    RAMIEL_TEST_ASSERT(vec_true);
}

RAMIEL_TEST_ADD(VecVectorTypecast) {
    Vec3f fvec = { 1.5f, 2.5f, 3.5f };
    Vec3  ivec = (Vec3)fvec;
    Vec3  expected = { 1, 2, 3 };
    RAMIEL_TEST_ASSERT(ivec == expected);
}

RAMIEL_TEST_ADD(VecVectorEqual) {
    Vec3 v1 = { 10, -1, 3 };
    Vec3 v2 = { 7, 5, -8 };
    Vec3 v3 = { 10, -1, 3 };
    RAMIEL_TEST_ASSERT(!(v1 == v2));
    RAMIEL_TEST_ASSERT(v1 == v3);
}

RAMIEL_TEST_ADD(VecVectorNotEqual) {
    Vec3 v1 = { 10, -1, 3 };
    Vec3 v2 = { 7, 5, -8 };
    Vec3 v3 = { 10, -1, 3 };
    RAMIEL_TEST_ASSERT(v1 != v2);
    RAMIEL_TEST_ASSERT(!(v1 != v3));
}

RAMIEL_TEST_ADD(VecVectorLt) {
    Vec3 v1 = { 4, 5, 6 };
    Vec3 v2 = { 5, 5, 7 };
    Vec3 v3 = { 5, 6, 7 };
    RAMIEL_TEST_ASSERT(!(v1 < v2));
    RAMIEL_TEST_ASSERT(v1 < v3);
}

RAMIEL_TEST_ADD(VecVectorGt) {
    Vec3 v1 = { 4, 5, 6 };
    Vec3 v2 = { 3, 5, 5 };
    Vec3 v3 = { 3, 4, 5 };
    RAMIEL_TEST_ASSERT(!(v1 > v2));
    RAMIEL_TEST_ASSERT(v1 > v3);
}

RAMIEL_TEST_ADD(VecVectorLte) {
    Vec3 v1 = { 4, 5, 6 };
    Vec3 v2 = { 3, 5, 7 };
    Vec3 v3 = { 4, 6, 6 };
    RAMIEL_TEST_ASSERT(!(v1 <= v2));
    RAMIEL_TEST_ASSERT(v1 <= v3);
}

RAMIEL_TEST_ADD(VecVectorGte) {
    Vec3 v1 = { 4, 5, 6 };
    Vec3 v2 = { 3, 5, 7 };
    Vec3 v3 = { 4, 4, 6 };
    RAMIEL_TEST_ASSERT(!(v1 >= v2));
    RAMIEL_TEST_ASSERT(v1 >= v3);
}

RAMIEL_TEST_ADD(VecScalarEqual) {
    Vec3 v = { 1, 1, 1 };
    RAMIEL_TEST_ASSERT(!(v == 2));
    RAMIEL_TEST_ASSERT(v == 1);
}

RAMIEL_TEST_ADD(VecScalarNotEqual) {
    Vec3 v = { 1, 1, 1 };
    RAMIEL_TEST_ASSERT(v != 2);
    RAMIEL_TEST_ASSERT(!(v != 1));
}

RAMIEL_TEST_ADD(VecScalarLt) {
    Vec3 v = { 2, 3, 4 };
    RAMIEL_TEST_ASSERT(!(v < 4));
    RAMIEL_TEST_ASSERT(v < 5);
}

RAMIEL_TEST_ADD(VecScalarGt) {
    Vec3 v = { 2, 3, 4 };
    RAMIEL_TEST_ASSERT(!(v > 2));
    RAMIEL_TEST_ASSERT(v > 1);
}

RAMIEL_TEST_ADD(VecScalarLte) {
    Vec3 v = { 2, 3, 4 };
    RAMIEL_TEST_ASSERT(!(v <= 3));
    RAMIEL_TEST_ASSERT(v <= 4);
}

RAMIEL_TEST_ADD(VecScalarGte) {
    Vec3 v = { 2, 3, 4 };
    RAMIEL_TEST_ASSERT(!(v >= 3));
    RAMIEL_TEST_ASSERT(v >= 2);
}

RAMIEL_TEST_ADD(VecVectorAssignment) {
    Vec3 original = { 1, -4, 2 };
    Vec3 copy = original;
    RAMIEL_TEST_ASSERT(copy == original);
}

RAMIEL_TEST_ADD(VecScalarAssignment) {
    Vec3 v;
    v = 5;
    Vec3 expected = { 5, 5, 5 };
    RAMIEL_TEST_ASSERT(v == expected);
}

RAMIEL_TEST_ADD(VecScalarAdditionAssignment) {
    Vec3 left = { -4, -9, -7 };
    int right = -9;
    Vec3 expected = { -13, -18, -16 };
    left += right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecScalarSubtractionAssignment) {
    Vec3 left = { 9, 6, -3 };
    int right = 5;
    Vec3 expected = { 4, 1, -8 };
    left -= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecScalarMultiplicationAssignment) {
    Vec3 left = { 0, -5, -1 };
    int right = 6;
    Vec3 expected = { 0, -30, -6 };
    left *= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecScalarDivisionAssignment) {
    Vec3 left = { -1, 8, 3 };
    int right = -3;
    Vec3 expected = { 0, -2, -1 };
    left /= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecVectorAdditionAssignment) {
    Vec3 left = { 7, 9, 2 };
    Vec3 right = { -5, 8, 0 };
    Vec3 expected = { 2, 17, 2 };
    left += right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecVectorSubtractionAssignment) {
    Vec3 left = { 7, -2, 9 };
    Vec3 right = { 6, -5, -2 };
    Vec3 expected = { 1, 3, 11 };
    left -= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecVectorMultiplicationaAssignment) {
    Vec3 left = { -6, -9, -8 };
    Vec3 right = { -7, 3, 7 };
    Vec3 expected = { 42, -27, -56 };
    left *= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecVectorDivisionAssignment) {
    Vec3 left = { 5, 5, 7 };
    Vec3 right = { 5, 2, 5 };
    Vec3 expected = { 1, 2, 1 };
    left /= right;
    RAMIEL_TEST_ASSERT(left == expected);
}

RAMIEL_TEST_ADD(VecScalarAddition) {
    Vec3 left = { 0, 8, -5 };
    int right = -4;
    Vec3 expected = { -4, 4, -9 };
    Vec3 res = left + right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecScalarSubtraction) {
    Vec3 left = { -2, -3, -1 };
    int right = 6;
    Vec3 expected = { -8, -9, -7 };
    Vec3 res = left - right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecScalarMultiplication) {
    Vec3 left = { 8, -8, -9 };
    int right = 7;
    Vec3 expected = { 56, -56, -63 };
    Vec3 res = left * right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecScalarDivision) {
    Vec3 left = { -2, -6, -5 };
    int right = 9;
    Vec3 expected = { 0, 0, 0 };
    Vec3 res = left / right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecVectorAddition) {
    Vec3 left = { -5, -6, -8 };
    Vec3 right = { 2, 1, -1 };
    Vec3 expected = { -3, -5, -9 };
    Vec3 res = left + right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecVectorSubtraction) {
    Vec3 left = { 3, 7, 8 };
    Vec3 right = { -7, -5, -3 };
    Vec3 expected = { 10, 12, 11 };
    Vec3 res = left - right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecVectorMultiplication) {
    Vec3 left = { 8, -7, 4 };
    Vec3 right = { 0, 4, -7 };
    Vec3 expected = { 0, -28, -28 };
    Vec3 res = left * right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecVectorDivision) {
    Vec3 left = { -2, 2, 8 };
    Vec3 right = { -6, 2, 6 };
    Vec3 expected = { 0, 1, 1 };
    Vec3 res = left / right;
    RAMIEL_TEST_ASSERT(res == expected);
}

RAMIEL_TEST_ADD(VecVectorNegate) {
    Vec3f v = { 0.2f, 5.5f, -5.7f };
    Vec3f res = -v;
    Vec3f expected = { -0.2f, -5.5f, 5.7f };
    RAMIEL_TEST_ASSERT(equal(res, expected));
}

RAMIEL_TEST_ADD(VecVectorFloatingPointEqual) {
    Vec2f v = { 1.0f, 1.00001f };

    Vec2f v1 = { 1.0f, 2.0f };
    Vec2f v2 = { 1.0f, 1.0f };
    RAMIEL_TEST_ASSERT(!equal(v, v1));
    RAMIEL_TEST_ASSERT(equal(v, v2));

    float s1 = 2.0f;
    float s2 = 1.0f;
    RAMIEL_TEST_ASSERT(!equal(v, s1));
    RAMIEL_TEST_ASSERT(equal(v, s2));
}

RAMIEL_TEST_ADD(VecVectorMagnitude) {
    Vec3f v = { -8.9f, -7.7f, -5.4f };
    float res = mag(v);
    float expected = 12.94836f;
    RAMIEL_TEST_ASSERT(std::abs(res - expected) < 0.0001f);
}

RAMIEL_TEST_ADD(VecVectorNormalize) {
    Vec3f v = { -5.2f, 3.3f, 8.5f };
    Vec3f res = normalize(v);
    Vec3f expected = { -0.495395f, 0.314386f, 0.809781f };
    RAMIEL_TEST_ASSERT(equal(res, expected));
}

RAMIEL_TEST_ADD(VecVectorNormalizeWithMagnitude) {
    Vec3f v = { -3.2f, -1.4f, 8.8f };
    Vec3f res = normalize(v, 9.46784f);
    Vec3f expected = { -0.33799f, -0.14786f, 0.92946f };
    RAMIEL_TEST_ASSERT(equal(res, expected));
}

RAMIEL_TEST_ADD(VecVectorDotProduct) {
    Vec3f v1 = { -9.2f, 6.9f, 9.9f };
    Vec3f v2 = { -8.2f, -3.5f, 4.6f };
    float res = dot(v1, v2);
    float expected = 96.83f;
    RAMIEL_TEST_ASSERT(std::abs(res - expected) < 0.0001f);
}

RAMIEL_TEST_ADD(VecVectorCrossProduct) {
    Vec3f v1 = { 9.1f, -4.9f, 0.4f };
    Vec3f v2 = { -5.0f, -8.2f, -7.3f };
    Vec3f res = cross(v1, v2);
    Vec3f expected = { 39.05f, 64.43f, -99.12f };
    RAMIEL_TEST_ASSERT(equal(res, expected));
}

RAMIEL_TEST_ADD(VecVectorMinScalar) {
    Vec3 vec = { 4, 5, 6 };
    Vec3 expected = { 4, 5, 5 };
    Vec3 actual = min(vec, 5);
    RAMIEL_TEST_ASSERT(expected == actual);
}

RAMIEL_TEST_ADD(VecVectorMinVector) {
    Vec3f vec = { 255.0f, 300.0f, 200.0f };
    Vec3f expected = { 255.0f, 255.0f, 200.0f };
    Vec3f actual = min(vec, Vec3f{ 255, 255, 255 });
    RAMIEL_TEST_ASSERT(expected == actual);
}

RAMIEL_TEST_ADD(VecVectorMaxScalar) {
    Vec3 vec = { 4, 5, 6 };
    Vec3 expected = { 5, 5, 6 };
    Vec3 actual = max(vec, 5);
    RAMIEL_TEST_ASSERT(expected == actual);
}

RAMIEL_TEST_ADD(VecVectorMaxVector) {
    Vec3f vec = { 255.0f, -5.0f, 100.0f };
    Vec3f expected = { 255.0f, 0.0f, 100.0f };
    Vec3f actual = max(vec, Vec3f());
    RAMIEL_TEST_ASSERT(expected == actual);
}
