#pragma once

#include "camera.h"

namespace ramiel {

    class Effect {
    public:
        virtual void run() const = 0;
    };


    class Brightness : public Effect {
        float brightness;
    public:
        Brightness(float brightness) : brightness(brightness) {}
        virtual void run() const override;
    };


    class ColorFilter : public Effect {
        Vec3f color;
    public:
        ColorFilter(Vec3f color) : color(color / 255.0f) {}
        virtual void run() const override;
    };


    class Contrast : public Effect {
        float contrast;
    public:
        Contrast(float contrast) : contrast(contrast) {}
        virtual void run() const override;
    };


    class Exposure : public Effect {
        float exposure;
    public:
        Exposure(float exposure) : exposure(exposure) {}
        virtual void run() const override;
    };


    class Fog : public Effect {
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
        virtual void run() const override;
    };


    class ToneMapping : public Effect {
    public:
        virtual void run() const override;
    };


    class Saturation : public Effect {
        float saturation;
    public:
        Saturation(float saturation) : saturation(saturation) {}
        virtual void run() const override;
    };

}
