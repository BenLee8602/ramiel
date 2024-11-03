#include <cmath>
#include "light.h"

namespace ramiel {

    Light::Light(Vec3f color, float intensity) {
        this->color = color;
        max(Vec3f(), this->color);
        this->intensity = std::max(0.0f, intensity);
        this->color *= this->intensity;
    }

    DirectionalLight::DirectionalLight(
        Vec3f color,
        float intensity,
        Vec3f dir
    ) : Light(color, intensity) {
        if (!dir) dir[Z] = 1.0f;
        this->dir = normalize(dir);
    }

    PointLight::PointLight(
        Vec3f color,
        float intensity,
        Vec3f pos,
        float falloff
    ) : Light(color, intensity) {
        if (falloff < 0.0f) falloff = 1.0f;
        this->pos = pos;
        this->falloff = falloff;
    }

    SpotLight::SpotLight(
        Vec3f color,
        float intensity,
        Vec3f pos,
        Vec3f dir,
        float falloff,
        float width,
        float falloffExp
    ) : PointLight(color, intensity, pos, falloff) {
        if (!dir[X] && !dir[Y] && !dir[Z]) dir[Z] = 1.0f;
        if (falloff < 0.0f) this->falloff = 0.1f;
        this->dir = normalize(dir);
        this->width = 1.0f - (width / 3.14159265f);
        this->falloffExp = falloffExp;
    }


    Vec3f DirectionalLight::getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        unsigned specularExponent,
        float specularIntensity
    ) const {
        // precalculate
        float d = dot(normal, this->dir);

        // diffuse
        float diffuse = std::max(0.0f, -d);

        // specular
        Vec3f cameraVec = normalize(getPos() - pos);
        Vec3f reflectedVec = this->dir - normal * d * 2.0f;
        float specular = specularIntensity * std::pow(
            std::max(0.0f, dot(reflectedVec, cameraVec)),
            specularExponent
        );

        return this->color * (diffuse + specular);
    }
    

    Vec3f PointLight::getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        unsigned specularExponent,
        float specularIntensity
    ) const {
        // precalculate
        Vec3f vec = pos - this->pos;
        float m = mag(vec);
        vec *= 1.0f / m; // normalize

        float d = dot(vec, normal);

        float falloff = 1.0f / (this->falloff * m * m + 1.0f);

        // diffuse
        float diffuse = std::max(0.0f, -d);

        // specular
        Vec3f cameraVec = normalize(getPos() - pos);
        Vec3f reflectedVec = vec - normal * d * 2.0f;
        float specular = specularIntensity * std::pow(
            std::max(0.0f, dot(reflectedVec, cameraVec)),
            specularExponent
        );

        return this->color * falloff * (diffuse + specular);
    }


    Vec3f SpotLight::getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        unsigned specularExponent,
        float specularIntensity
    ) const {
        // precalculate
        Vec3f vec = pos - this->pos;
        float m = mag(vec);
        vec /= m; // normalize

        float s = std::max(0.0f, dot(vec, this->dir));
        if (s < width) return Vec3f();

        float d = dot(vec, normal);

        float falloff = 1.0f / (this->falloff * m * m + 1.0f) * std::pow(s, this->falloffExp);

        // diffuse
        float diffuse = std::max(0.0f, -d);

        // specular
        Vec3f cameraVec = normalize(getPos() - pos);
        Vec3f reflectedVec = vec - normal * d * 2.0f;
        float specular = specularIntensity * std::pow(
            std::max(0.0f, dot(reflectedVec, cameraVec)),
            specularExponent
        );

        return this->color * falloff * (diffuse + specular);
    }

}
