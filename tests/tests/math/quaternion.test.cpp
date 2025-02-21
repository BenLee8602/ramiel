#include <ramiel/test.h>
#include <ramiel/math.h>
using namespace ramiel;

RAMIEL_TEST_ADD(QtnQtn) {
    const Quaternion q1 = {};
    const Quaternion q2 = {};

    const Quaternion qExpected = {};
    const Quaternion qActual = qtnqtn(q1, q2);

    RAMIEL_TEST_ASSERT(equal(qExpected, qActual));
}


RAMIEL_TEST_ADD(QtnVec) {
    const Quaternion q = {};
    const Vec3f v = {};

    const Vec3f vExpected = {};
    const Vec3f vActual = qtnvec(q, v);

    RAMIEL_TEST_ASSERT(equal(vExpected, vActual));
}


RAMIEL_TEST_ADD(QtnConj) {
    const Quaternion q = {};

    const Quaternion qExpected = {};
    const Quaternion qActual = qtnconj(q);

    RAMIEL_TEST_ASSERT(equal(qExpected, qActual));
}


RAMIEL_TEST_ADD(QtnInv) {
    const Quaternion q = {};

    const Quaternion qExpected = {};
    const Quaternion qActual = qtninv(q);

    //RAMIEL_TEST_ASSERT(equal(qExpected, qActual));
}


RAMIEL_TEST_ADD(QtnSlerp) {
    const Quaternion q1 = {};
    const Quaternion q2 = {};
    const float t = 0.707f;

    const Quaternion qExpected = {};
    const Quaternion qActual = qtnslerp(q1, q2, t);
    
    RAMIEL_TEST_ASSERT(equal(qExpected, qActual));
}
