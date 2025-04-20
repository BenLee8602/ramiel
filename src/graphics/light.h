#pragma once

#include <memory>
#include "camera.h"

namespace ramiel {

    class Light;

    void addLight(Light* light);
    void removeLight(Light* light);

    Vec3f getAmbientLight();
    void setAmbientLight(const Vec3f& color);

    Vec3f getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        float specularExponent = 0.0f,
        float specularIntensity = 0.0f
    );


    class Light {
    public:
        Light(Vec3f color, float intensity);

        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            unsigned specularExponent,
            float specularIntensity
        ) const = 0;

        Vec3f getColor() const;
        float getIntensity() const;

        void setColor(Vec3f color);
        void setIntensity(float intensity);

    protected:
        Vec3f hue;
        float intensity;
        Vec3f color;
    };


    class DirectionalLight : public Light {
    public:
        DirectionalLight(Vec3f color, float intensity, Vec3f dir);

        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            unsigned specularExponent,
            float specularIntensity
        ) const override;

        Vec3f getDir() const;

        void setDir(Vec3f dir);

    protected:
        Vec3f dir;
    };


    class PointLight : public Light {
    public:
        PointLight(Vec3f color, float intensity, Vec3f pos, float falloff);

        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            unsigned specularExponent,
            float specularIntensity
        ) const override;

        Vec3f getPos() const;
        float getFalloff() const;

        void setPos(Vec3f pos);
        void setFalloff(float falloff);

    protected:
        Vec3f pos;
        float falloff;
    };


    class SpotLight : public PointLight {
    public:
        SpotLight(
            Vec3f color,
            float intensity,
            Vec3f pos,
            Vec3f dir,
            float falloff,
            float width,
            float falloffExp
        );

        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            unsigned specularExponent,
            float specularIntensity
        ) const override;

        Vec3f getDir() const;
        float getWidth() const;
        float getFalloffExp() const;

        void setDir(Vec3f dir);
        void setWidth(float width);
        void setFalloffExp(float falloffExp);
        
    protected:
        Vec3f dir;
        float width;
        float falloffExp;
    };

}
