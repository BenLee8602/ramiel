#pragma once

#include <vector>
#include "vec.h"

namespace ramiel {

    class Texture {
        Vec2 size;
        size_t maxIdx;
        std::vector<Vec3f> data;
    public:
        Texture(std::string filename, char type = 'c');
        Vec3f get(const Vec2f& coords) const;
        Vec2 getSize() const;
    };

}
