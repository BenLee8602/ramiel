
#include <ramiel/test.h>
#include <ramiel/data.h>
using namespace ramiel;


int count;
struct InstanceCount {
    InstanceCount() { count++; }
    ~InstanceCount() { count--; }
};

RAMIEL_TEST_ADD(VoidBufferConstructorDestructor) {
    count = 0;
    {
        RAMIEL_TEST_ASSERT(count == 0);
        VoidBuffer vb = makeVoidBuffer<InstanceCount>(64);
        RAMIEL_TEST_ASSERT(count == 64);
    }
    RAMIEL_TEST_ASSERT(count == 0);
}


RAMIEL_TEST_ADD(VoidBufferCopyConstructor) {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest(src);

    RAMIEL_TEST_ASSERT(dest.getType() == src.getType());
    RAMIEL_TEST_ASSERT(dest.getSize() == src.getSize());
    RAMIEL_TEST_ASSERT(dest.getLength() == src.getLength());

    for (size_t i = 0; i < length; ++i)
        RAMIEL_TEST_ASSERT(c(dest[i]) == c(src[i]));
    
    c(dest[0]) = -1;
    RAMIEL_TEST_ASSERT(c(dest[0]) != c(src[0]));
}


RAMIEL_TEST_ADD(VoidBufferMoveConstructor) {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest(std::move(src));

    RAMIEL_TEST_ASSERT(dest.getType() == typeid(int));
    RAMIEL_TEST_ASSERT(dest.getSize() == sizeof(int));
    RAMIEL_TEST_ASSERT(dest.getLength() == length);

    for (size_t i = 0; i < length; ++i)
        RAMIEL_TEST_ASSERT(c(dest[i]) == i);
    
    RAMIEL_TEST_ASSERT(src.getLength() == 0);
}


RAMIEL_TEST_ADD(VoidBufferCopyAssignment) {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest = makeVoidBuffer<double>(8);
    dest = src;

    RAMIEL_TEST_ASSERT(dest.getType() == src.getType());
    RAMIEL_TEST_ASSERT(dest.getSize() == src.getSize());
    RAMIEL_TEST_ASSERT(dest.getLength() == src.getLength());

    for (size_t i = 0; i < length; ++i)
        RAMIEL_TEST_ASSERT(c(dest[i]) == c(src[i]));
    
    c(dest[0]) = -1;
    RAMIEL_TEST_ASSERT(c(dest[0]) != c(src[0]));
}


RAMIEL_TEST_ADD(VoidBufferMoveAssignment) {
    constexpr size_t length = 4;
    auto c = [](void* v) -> int& { return *static_cast<int*>(v); };

    VoidBuffer src = makeVoidBuffer<int>(length);
    for (size_t i = 0; i < length; ++i)
        c(src[i]) = i;
    
    VoidBuffer dest = makeVoidBuffer<double>(8);
    dest = std::move(src);

    RAMIEL_TEST_ASSERT(dest.getType() == typeid(int));
    RAMIEL_TEST_ASSERT(dest.getSize() == sizeof(int));
    RAMIEL_TEST_ASSERT(dest.getLength() == length);

    for (size_t i = 0; i < length; ++i)
        RAMIEL_TEST_ASSERT(c(dest[i]) == i);
    
    RAMIEL_TEST_ASSERT(src.getLength() == 0);
}


RAMIEL_TEST_ADD(VoidBufferGetType) {
    VoidBuffer vb = makeVoidBuffer<char>(32);
    std::type_index expected = typeid(char);
    std::type_index actual = vb.getType();
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(VoidBufferGetSize) {
    VoidBuffer vb = makeVoidBuffer<int16_t>(16);
    size_t expected = sizeof(int16_t);
    size_t actual = vb.getSize();
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(VoidBufferGetLength) {
    VoidBuffer vb = makeVoidBuffer<char>();
    RAMIEL_TEST_ASSERT(vb.getLength() == 0);

    size_t expected = 8;
    vb.setLength(expected);
    size_t actual = vb.getLength();
    RAMIEL_TEST_ASSERT(expected == actual);
}


RAMIEL_TEST_ADD(VoidBufferIterate) {
    VoidBuffer vb = makeVoidBuffer<int>(4);

    for (size_t i = 0; i < vb.getLength(); ++i)
        *static_cast<int*>(vb[i]) = i;
    
    int* it = static_cast<int*>(vb[0]);
    for (size_t i = 0; i < vb.getLength(); ++i, ++it)
        RAMIEL_TEST_ASSERT(*it == i);
}
