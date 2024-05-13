#pragma once

#include <vector>
#include <string>
#include <ramiel/math.h>

namespace ramiel {

    class Texture {
        Vec2u res;
        Vec2u resClamp;
        std::vector<Vec3f> data;
    public:
        Texture(std::vector<Vec3f>& data, Vec2u res);
        Texture(const std::string& filename, Mat4x4f format = id<float, 4>());
        Vec3f get(const Vec2f& coords) const;
        Vec2u getSize() const;
    };

}
