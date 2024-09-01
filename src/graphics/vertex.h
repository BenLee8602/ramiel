#pragma once

#include <type_traits>
#include <ramiel/math.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winvalid-offsetof"

namespace ramiel {

    template<class T, typename = void>
    struct IsValidVertex : std::false_type {};

    template<class T>
    struct IsValidVertex<T, std::void_t<decltype(T{}.pos)>>
    : std::conjunction<
        std::is_aggregate<T>,
        std::is_same<Vec4f, decltype(T{}.pos)>,
        std::bool_constant<(offsetof(T, pos) == 0)>
    > {};

    template<class T>
    void assertValidVertex() {
        static_assert(
            IsValidVertex<T>::value,
            "\na valid vertex type must satisfy the following:\n"
            "    1. must be an aggregate type\n"
            "    2. must contain the field \"pos\"\n"
            "    3. \"pos\" must be of type \"ramiel::Vec4f\"\n"
            "    4. \"pos\" must be the first field\n"
        );
    }

}

#pragma GCC diagnostic pop
