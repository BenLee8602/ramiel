#pragma once

#include <functional>
#include <ramiel/math.h>

namespace ramiel {

    using PixelFn = std::function<void(const Vec2f&)>;
    
    void rasterizeTri(
        const Vec4f& v0,
        const Vec4f& v1,
        const Vec4f& v2,
        PixelFn pixelFn
    );

}
