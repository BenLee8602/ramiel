#pragma once

#include <vector>
#include "vec.h"

namespace ramiel {

    class Texture {
        Vec2 size;
        size_t maxIdx;
    protected:
        std::vector<Vec3f> data;
    public:
        Texture(std::string filename);
        Vec3f get(const Vec2f& coords) const;
        Vec2 getSize() const;
    };

    class TextureRGB : public Texture {
    public:
        TextureRGB(std::string filename);
    };

    class TextureNML : public Texture {
    public:
        TextureNML(std::string filename);
    };

}
