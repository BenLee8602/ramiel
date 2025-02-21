#include <algorithm>
#include "effects.h"

namespace ramiel {

    Vec3f Brightness::operator()(const Vec3f& pixel) const {
        return min(max(pixel + brightness, 0.0f), 255.0f);
    }


    Vec3f Contrast::operator()(const Vec3f& pixel) const {
        Vec3f out = (pixel - 127.5f) * contrast + 127.5f;
        return min(max(out, 0.0f), 255.0f);
    }


    Vec3f Exposure::operator()(const Vec3f& pixel) const {
        return min(pixel * exposure, 255.0f);
    }


    Vec3f Fog::operator()(const Vec3f& pixel, float depth) const {
        if (depth <= fogStart) return pixel;
        if (depth >= fogEnd) return fogColor;
        float shift = (depth - fogStart) * fogFactor;
        return pixel * (1.0f - shift) + fogColor * shift;
    }


    Vec3f Saturation::operator()(const Vec3f& pixel) const {
        static constexpr Vec3f rgbToLum = {
            0.2126f,
            0.7152f,
            0.0722f
        };

        float lum = dot(pixel, rgbToLum);
        Vec3f out = pixel * saturation + lum * (1.0f - saturation);
        return min(max(out, 0.0f), 255.0f);
    }

}
