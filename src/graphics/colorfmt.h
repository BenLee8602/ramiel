#pragma once

#include <functional>
#include <ramiel/math.h>

namespace ramiel {

    using ColorFormat = std::function<void(const Vec3f&, void*)>;

    void cfmtRGB888(const Vec3f& in, void* out);
    void cfmtBGR888(const Vec3f& in, void* out);
    void cfmtRGBA8888(const Vec3f& in, void* out);
    void cfmtBGRA8888(const Vec3f& in, void* out);
    void cfmtARGB8888(const Vec3f& in, void* out);
    void cfmtABGR8888(const Vec3f& in, void* out);

}

