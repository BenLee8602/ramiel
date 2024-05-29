
#include <catch2/catch2.hpp>
#include <ramiel/data.h>
using namespace ramiel;


int count;
struct InstanceCount {
    InstanceCount() { count++; }
    ~InstanceCount() { count--; }
};

TEST_CASE("proper construction and destruction", "[voidbuffer]") {
    count = 0;
    {
        REQUIRE(count == 0);
        VoidBuffer vb = makeVoidBuffer<InstanceCount>(64);
        REQUIRE(count == 64);
    }
    REQUIRE(count == 0);
}


TEST_CASE("get void buffer type", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<char>(32);
    std::type_index expected = typeid(char);
    std::type_index actual = vb.getType();
    REQUIRE(expected == actual);
}


TEST_CASE("get void buffer size", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<int16_t>(16);
    size_t expected = sizeof(int16_t);
    size_t actual = vb.getSize();
    REQUIRE(expected == actual);
}


TEST_CASE("void buffer length", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<char>();
    REQUIRE(vb.getLength() == 0);

    size_t expected = 8;
    vb.setLength(expected);
    size_t actual = vb.getLength();
    REQUIRE(expected == actual);
}


TEST_CASE("iterate void buffer", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<int>(4);

    for (size_t i = 0; i < vb.getLength(); ++i)
        *static_cast<int*>(vb[i]) = i;
    
    int* it = static_cast<int*>(vb[0]);
    for (size_t i = 0; i < vb.getLength(); ++i, ++it)
        REQUIRE(*it == i);
}
