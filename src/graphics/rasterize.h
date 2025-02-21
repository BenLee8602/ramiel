#pragma once

#include <functional>
#include <ramiel/math.h>

namespace ramiel {

    using TriPixelFn = std::function<void(const Vec2f&)>;
    using LinePixelFn = std::function<void(const Vec2f&, float)>;
    
    void rasterizeTri(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        TriPixelFn pixelFn
    );

    void rasterizeLine(
        const Vec4f& v0,
        const Vec4f& v1,
        LinePixelFn pixelFn
    );

}
