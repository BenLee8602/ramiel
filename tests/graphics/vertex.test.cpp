#include <catch2/catch2.hpp>
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


TEST_CASE("validate vertex type", "[vertex]") {
    REQUIRE_FALSE(IsValidVertex<Type1>::value);
    REQUIRE_FALSE(IsValidVertex<Type2>::value);
    REQUIRE_FALSE(IsValidVertex<Type3>::value);
    REQUIRE_FALSE(IsValidVertex<Type4>::value);
    REQUIRE(IsValidVertex<Type5>::value);
}
