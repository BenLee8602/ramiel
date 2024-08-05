#include <catch2/catch2.hpp>
#include <ramiel/data.h>
using namespace ramiel;


struct TestStruct {
    int m1;
    double m2;
    char m3;
};

using TestTuple = AsTuple<TestStruct>;


TEST_CASE("tuple get", "[tuple]") {
    TestTuple tuple;
    tuple.get<0>() = 5;
    tuple.get<1>() = 2.3;
    tuple.get<2>() = 127;
    
    const TestTuple& ctuple = tuple;
    REQUIRE(ctuple.get<0>() == 5);
    REQUIRE(ctuple.get<1>() == 2.3);
    REQUIRE(ctuple.get<2>() == 127);
}


TEST_CASE("tuple addition", "[tuple]") {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    TestTuple r = makeTuple(1, 0.7, (char)1);

    TestTuple res = l + r;
    REQUIRE(res.get<0>() == 6);
    REQUIRE(std::abs(res.get<1>() - 3.0) < 0.0001);
    REQUIRE(res.get<2>() == -128);
}


TEST_CASE("tuple subtraction", "[tuple]") {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    TestTuple r = makeTuple(1, 0.7, (char)1);

    TestTuple res = l - r;
    REQUIRE(res.get<0>() == 4);
    REQUIRE(std::abs(res.get<1>() - 1.6) < 0.0001);
    REQUIRE(res.get<2>() == 126);
}


TEST_CASE("tuple multiplication", "[tuple]") {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    constexpr float r = 2.0f;

    TestTuple res = l * r;
    REQUIRE(res.get<0>() == 10);
    REQUIRE(std::abs(res.get<1>() - 4.6) < 0.0001);
    REQUIRE(res.get<2>() == -2);
}


TEST_CASE("tuple division", "[tuple]") {
    TestTuple l = makeTuple(5, 2.3, (char)127);
    constexpr float r = 2.0f;

    TestTuple res = l / r;
    REQUIRE(res.get<0>() == 2);
    REQUIRE(std::abs(res.get<1>() - 1.15) < 0.0001);
    REQUIRE(res.get<2>() == 63);

}


TEST_CASE("struct to tuple", "[tuple]") {
    TestStruct s = { 5, 2.3, 'p' };
    TestTuple& t = reinterpret_cast<TestTuple&>(s);

    REQUIRE(t.get<0>() == 5);
    REQUIRE(t.get<1>() == 2.3);
    REQUIRE(t.get<2>() == 'p');
}
