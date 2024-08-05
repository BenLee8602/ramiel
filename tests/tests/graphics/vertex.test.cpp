#include <ramiel/test.h>
#include <ramiel/graphics.h>
using namespace ramiel;


// not aggregate
struct Type1 {
    Vec4f pos;
private:
    float m1;
};

// wrong field name
struct Type2 {
    Vec4f m1;
};

// wrong field type
struct Type3 {
    float pos;
};

// wrong field offset
struct Type4 {
    float m1;
    Vec4f pos;
};

// all good!
struct Type5 {
    Vec4f pos;
};


RAMIEL_TEST_ADD(VertexValidate) {
    RAMIEL_TEST_ASSERT(IsValidVertex<Type1>::value == false);
    RAMIEL_TEST_ASSERT(IsValidVertex<Type2>::value == false);
    RAMIEL_TEST_ASSERT(IsValidVertex<Type3>::value == false);
    RAMIEL_TEST_ASSERT(IsValidVertex<Type4>::value == false);
    RAMIEL_TEST_ASSERT(IsValidVertex<Type5>::value == true);
}
