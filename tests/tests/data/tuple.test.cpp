#include <ramiel/test.h>
#include <ramiel/data.h>
using namespace ramiel;


struct TestStruct {
    int m1;
    double m2;
    char m3;
};

using TestTuple = AsTuple<TestStruct>;


RAMIEL_TEST_ADD(TupleGet) {
    TestTuple tuple;
    tuple.get<0>() = 5;
    tuple.get<1>() = 2.3;
    tuple.get<2>() = 127;
    
    const TestTuple& ctuple = tuple;
    RAMIEL_TEST_ASSERT(ctuple.get<0>() == 5);
    RAMIEL_TEST_ASSERT(ctuple.get<1>() == 2.3);
    RAMIEL_TEST_ASSERT(ctuple.get<2>() == 127);
}


RAMIEL_TEST_ADD(TupleAddition) {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    TestTuple r = makeTuple(1, 0.7, (char)1);

    TestTuple res = l + r;
    RAMIEL_TEST_ASSERT(res.get<0>() == 6);
    RAMIEL_TEST_ASSERT(std::abs(res.get<1>() - 3.0) < 0.0001);
    RAMIEL_TEST_ASSERT(res.get<2>() == -128);
}


RAMIEL_TEST_ADD(TupleSubtraction) {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    TestTuple r = makeTuple(1, 0.7, (char)1);

    TestTuple res = l - r;
    RAMIEL_TEST_ASSERT(res.get<0>() == 4);
    RAMIEL_TEST_ASSERT(std::abs(res.get<1>() - 1.6) < 0.0001);
    RAMIEL_TEST_ASSERT(res.get<2>() == 126);
}


RAMIEL_TEST_ADD(TupleMultiplication) {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    constexpr float r = 2.0f;

    TestTuple res = l * r;
    RAMIEL_TEST_ASSERT(res.get<0>() == 10);
    RAMIEL_TEST_ASSERT(std::abs(res.get<1>() - 4.6) < 0.0001);
    RAMIEL_TEST_ASSERT(res.get<2>() == -2);
}


RAMIEL_TEST_ADD(TupleDivision) {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    constexpr float r = 2.0f;

    TestTuple res = l / r;
    RAMIEL_TEST_ASSERT(res.get<0>() == 2);
    RAMIEL_TEST_ASSERT(std::abs(res.get<1>() - 1.15) < 0.0001);
    RAMIEL_TEST_ASSERT(res.get<2>() == 63);

}


RAMIEL_TEST_ADD(TupleFromStruct) {
    TestStruct s = { 5, 2.3, 'p' };
    TestTuple& t = reinterpret_cast<TestTuple&>(s);

    RAMIEL_TEST_ASSERT(t.get<0>() == 5);
    RAMIEL_TEST_ASSERT(t.get<1>() == 2.3);
    RAMIEL_TEST_ASSERT(t.get<2>() == 'p');
}
