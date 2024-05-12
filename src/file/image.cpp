#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "image.h"

namespace ramiel {

    bool loadImage(
        const std::string& filename,
        Vec2u& res,
        std::vector<Vec3f>& data,
        Mat4x4f format
    ) {
        int x, y, comp;
        uint8_t* rawData = stbi_load(filename.c_str(), &x, &y, &comp, 3);
        if (!rawData) return false;

        res = { (unsigned)x, (unsigned)y };
        data = std::vector<Vec3f>(res[X] * res[Y]);

        uint8_t* d = rawData;
        for (size_t i = 0; i < res[X] * res[Y]; ++i) {
            Vec4f in = { (float)*d++, (float)*d++, (float)*d++, 1.0f };
            Vec4f out = matvec(format, in);
            data[i] = { out[0], out[1], out[2] };
        }

        stbi_image_free(rawData);
        return true;
    }

}
