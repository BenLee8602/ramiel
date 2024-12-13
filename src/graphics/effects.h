#pragma once

#include "camera.h"

namespace ramiel {

    struct Brightness {
        float brightness;
        Vec3f operator()(const Vec3f& pixel) const;
    };


    struct Contrast {
        float contrast;
        Vec3f operator()(const Vec3f& pixel) const;
    };


    struct Exposure {
        float exposure;
        Vec3f operator()(const Vec3f& pixel) const;
    };


    class Fog {
        Vec3f fogColor;
        float fogStart;
        float fogEnd;
        float fogFactor;
    public:
        Fog(
            Vec3f fogColor,
            float fogStart,
            float fogEnd
        ) :
            fogColor(fogColor),
            fogStart(fogStart),
            fogEnd(fogEnd),
            fogFactor(1.0f / (fogEnd - fogStart))
        {}
        Vec3f operator()(const Vec3f& pixel, float depth) const;
    };


    struct Saturation {
        float saturation;
        Vec3f operator()(const Vec3f& pixel) const;
    };

}
