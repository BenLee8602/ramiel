#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include "texture.h"

namespace ramiel {

    Texture::Texture(const char* filename) {
        uint8_t* buffer;
        int bitsPerPixel;
        buffer = stbi_load(filename, &size[X], &size[Y], &bitsPerPixel, 3);
        data = std::vector<Vec3f>(size[X] * size[Y]);
        maxIdx = data.size() - 1;
        uint8_t* b = buffer;
        for (size_t i = 0; i < data.size(); ++i) {
            data[i][R] = *b++;
            data[i][G] = *b++;
            data[i][B] = *b++;
            data[i] /= 255.0f;
        }
        delete[] buffer;
    }


    Vec3f Texture::get(const Vec2f& coords) const {
        Vec2 unnormalized = size * coords;
        size_t i = unnormalized[X] + unnormalized[Y] * size[X];
        return data[std::min(i, maxIdx)];
    }


    Vec2 Texture::getSize() const {
        return size;
    }

}
