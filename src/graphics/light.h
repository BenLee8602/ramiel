#pragma once

#include "camera.h"

namespace ramiel {

    class Light {
    protected:
        Vec3f color;
        float intensity;
    public:
        Light(Vec3f color, float intensity);
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal
        ) const = 0;
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            const Vec3f& cameraPos,
            unsigned specularExponent,
            float specularIntensity
        ) const = 0;
    };

    class DirectionalLight : public Light {
    protected:
        Vec3f dir;
    public:
        DirectionalLight(Vec3f color, float intensity, Vec3f dir);
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal
        ) const override;
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            const Vec3f& cameraPos,
            unsigned specularExponent,
            float specularIntensity
        ) const override;
    };

    class PointLight : public Light {
    protected:
        Vec3f pos;
        float falloff;
    public:
        PointLight(Vec3f color, float intensity, Vec3f pos, float falloff);
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal
        ) const override;
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            const Vec3f& cameraPos,
            unsigned specularExponent,
            float specularIntensity
        ) const override;
    };

    class SpotLight : public PointLight {
        Vec3f dir;
        float width;
        float falloffExp;
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
            const Vec3f& normal
        ) const override;
        virtual Vec3f getLight(
            const Vec3f& pos,
            const Vec3f& normal,
            const Vec3f& cameraPos,
            unsigned specularExponent,
            float specularIntensity
        ) const override;
    };


    class LightingList {
    protected:
        const Vec3f& ambientLight;
        const std::vector<Light*>& lights;
    public:
        LightingList(
            const Vec3f& ambientLight,
            const std::vector<Light*>& lights
        ) : 
            ambientLight(ambientLight),
            lights(lights)
        {}
        virtual Vec3f getAllLight(
            const Vec3f& pos,
            const Vec3f& normal
        ) const;
    };

    class LightingListSpecular : public LightingList {
        const Vec3f& cameraPos;
        uint16_t specularExponent;
        float specularIntensity;
    public:
        LightingListSpecular(
            const Vec3f& ambientLight,
            const std::vector<Light*>& lights,
            const Vec3f& cameraPos,
            uint16_t specularExponent,
            float specularIntensity
        ) :
            LightingList(ambientLight, lights),
            cameraPos(cameraPos),
            specularExponent(specularExponent),
            specularIntensity(specularIntensity)
        {}
        virtual Vec3f getAllLight(
            const Vec3f& pos,
            const Vec3f& normal
        ) const override;
    };

}
