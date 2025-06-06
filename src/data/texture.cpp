#include <ramiel/file.h>
#include "texture.h"

namespace ramiel {

    Texture::Texture(std::vector<Vec3f>&& data, Vec2u res)
        : res(res)
        , resClamp(res - 1)
        , data(std::move(data))
    {}

    Texture::Texture(const std::vector<Vec3f>& data, Vec2u res)
        : res(res)
        , resClamp(res - 1)
        , data(data)
    {}

    Texture::Texture(const std::string& filename, Mat4x4f format) {
        res = Vec2u();
        loadImage(filename, res, data, format);
        resClamp = res - 1;
    }


    Vec3f Texture::get(const Vec2f& coords) const {
        Vec2u unnormalized = min(res * coords, resClamp);
        return data[unnormalized[X] + unnormalized[Y] * res[X]];
    }


    Vec2u Texture::getSize() const {
        return res;
    }

    Vec3f* Texture::getData() {
        return data.data();
    }

}
