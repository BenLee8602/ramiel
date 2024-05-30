
#include <catch2/catch2.hpp>
#include <ramiel/data.h>
using namespace ramiel;


int count;
struct InstanceCount {
    InstanceCount() { count++; }
    ~InstanceCount() { count--; }
};

TEST_CASE("void buffer construction and destruction", "[voidbuffer]") {
    count = 0;
    {
        REQUIRE(count == 0);
        VoidBuffer vb = makeVoidBuffer<InstanceCount>(64);
        REQUIRE(count == 64);
    }
    REQUIRE(count == 0);
}


TEST_CASE("void buffer copy constructor", "[voidbuffer]") {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest(src);

    REQUIRE(dest.getType() == src.getType());
    REQUIRE(dest.getSize() == src.getSize());
    REQUIRE(dest.getLength() == src.getLength());

    for (size_t i = 0; i < length; ++i)
        REQUIRE(c(dest[i]) == c(src[i]));
    
    c(dest[0]) = -1;
    REQUIRE(c(dest[0]) != c(src[0]));
}


TEST_CASE("void buffer move constructor", "[voidbuffer]") {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest(std::move(src));

    REQUIRE(dest.getType() == typeid(int));
    REQUIRE(dest.getSize() == sizeof(int));
    REQUIRE(dest.getLength() == length);

    for (size_t i = 0; i < length; ++i)
        REQUIRE(c(dest[i]) == i);
    
    REQUIRE(src.getLength() == 0);
}


TEST_CASE("void buffer copy assignment", "[voidbuffer]") {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest = makeVoidBuffer<double>(8);
    dest = src;

    REQUIRE(dest.getType() == src.getType());
    REQUIRE(dest.getSize() == src.getSize());
    REQUIRE(dest.getLength() == src.getLength());

    for (size_t i = 0; i < length; ++i)
        REQUIRE(c(dest[i]) == c(src[i]));
    
    c(dest[0]) = -1;
    REQUIRE(c(dest[0]) != c(src[0]));
}


TEST_CASE("void buffer move assignment", "[voidbuffer]") {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest = makeVoidBuffer<double>(8);
    dest = std::move(src);

    REQUIRE(dest.getType() == typeid(int));
    REQUIRE(dest.getSize() == sizeof(int));
    REQUIRE(dest.getLength() == length);

    for (size_t i = 0; i < length; ++i)
        REQUIRE(c(dest[i]) == i);
    
    REQUIRE(src.getLength() == 0);
}


TEST_CASE("void buffer get type", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<char>(32);
    std::type_index expected = typeid(char);
    std::type_index actual = vb.getType();
    REQUIRE(expected == actual);
}


TEST_CASE("void buffer get size", "[voidbuffer]") {
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


TEST_CASE("void buffer iterate", "[voidbuffer]") {
    VoidBuffer vb = makeVoidBuffer<int>(4);

    for (size_t i = 0; i < vb.getLength(); ++i)
        *static_cast<int*>(vb[i]) = i;
    
    int* it = static_cast<int*>(vb[0]);
    for (size_t i = 0; i < vb.getLength(); ++i, ++it)
        REQUIRE(*it == i);
}
