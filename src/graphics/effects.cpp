#include <algorithm>
#include "effects.h"

namespace ramiel {

    void Brightness::run(Camera& camera) const {
        auto color = camera.getColorBuffer();
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            color[i] += brightness;
            color[i] = min(max(color[i], 0.0f), 255.0f);
        }
    }


    void ColorFilter::run(Camera& camera) const {
        auto color = camera.getColorBuffer();
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            color[i] *= this->color;
            color[i] = min(color[i], 255.0f);
        }
    };


    void Contrast::run(Camera& camera) const {
        auto color = camera.getColorBuffer();
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            color[i] = (color[i] - 127.5f) * contrast + 127.5f;
            color[i] = min(max(color[i], 0.0f), 255.0f);
        }
    }


    void Exposure::run(Camera& camera) const {
        auto color = camera.getColorBuffer();
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            color[i] *= exposure;
            color[i] = min(color[i], 255.0f);
        }
    }


    void Fog::run(Camera& camera) const {
        auto color = camera.getColorBuffer();
        auto depth = camera.getDepthBuffer();

        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            if (depth[i] <= fogStart) continue;
            if (depth[i] >= fogEnd) {
                color[i] = fogColor;
                continue;
            }
            float shift = (depth[i] - fogStart) * fogFactor;
            color[i] = color[i] * (1.0f - shift) + fogColor * shift;
        }
    }




    static inline float luminance(Vec3f rgb) {
        constexpr Vec3f rgbToLum = {
            0.2126f / 255.0f,
            0.7152f / 255.0f,
            0.0722f / 255.0f
        };
        return dot(rgb, rgbToLum);
    }

    static inline void changeLuminance(Vec3f& rgb, float oldLum, float newLum) {
        rgb *= newLum / oldLum;
    }

    static inline void changeLuminance(Vec3f& rgb, float lum) {
        changeLuminance(rgb, luminance(rgb), lum);
    }

    // extended reinhard applied to luminance
    void ToneMapping::run(Camera& camera) const {
        auto color = camera.getColorBuffer();

        float maxLum = luminance(*std::max_element(
            color, color + camera.getBufferSize(),
            [](const Vec3f& p1, const Vec3f& p2) {
                return luminance(p1) < luminance(p2);
            }
        ));

        if (maxLum <= 1.0f) return;

        float ml = 1.0f / (maxLum * maxLum);
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            float oldLum = luminance(color[i]);
            float newLum = oldLum * (1.0f + oldLum * ml) / (1.0f + oldLum);
            changeLuminance(color[i], oldLum, newLum);
        }
    }


    void Saturation::run(Camera& camera) const {
        constexpr Vec3f rgbToLum = {
            0.2126f,
            0.7152f,
            0.0722f
        };

        auto color = camera.getColorBuffer();
        for (size_t i = 0; i < camera.getBufferSize(); ++i) {
            float lum = dot(color[i], rgbToLum);
            Vec3f grey = { lum, lum, lum };
            color[i] = color[i] * saturation + grey * (1.0f - saturation);
            color[i] = min(max(color[i], 0.0f), 255.0f);
        }
    }

}
