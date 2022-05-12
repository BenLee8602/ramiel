#pragma once

#include <vector>
#include "vec.h"

namespace bl {

    class Texture {
        Vec2 size;
        size_t maxIdx;
        std::vector<Vec3f> data;
    public:
        Texture(const char* filename);
        Vec3f get(const Vec2f& coords) const;
        Vec2 getSize() const;
    };

}
