#pragma once

#include <array>
#include <list>

#include <ramiel/math.h>

namespace ramiel {

    using Tri = std::array<Vec4f, 3>;
    using TriList = std::list<Tri>;

    bool clip(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriList& clippedTris
    );

    bool clipLine(Vec4f& v0, Vec4f& v1);

}
