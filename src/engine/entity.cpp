#include "entity.h"
#include "graphics.h"
#include "task.h"
#include "serialize.h"
using namespace ramiel;

namespace {

    bool setLightProp(std::string prop, std::string val, Light& light) {
        if (prop == "color") {
            Vec3f color;
            if (!fromString(val, color)) return false;
            if (color < 0.0f) return false;
            light.setColor(color);
            return true;
        }
        if (prop == "intensity") {
            float intensity;
            if (!fromString(val, intensity)) return false;
            if (intensity < 0.0f) return false;
            light.setIntensity(intensity);
            return true;
        }
        return false;
    }

    bool setDirLightProp(std::string prop, std::string val, DirectionalLight& light) {
        if (setLightProp(prop, val, light)) return true;
        if (prop == "dir") {
            Vec3f dir;
            if (!fromString(val, dir)) return false;
            if (!dir) return false;
            light.setDir(dir);
            return true;
        }
        return false;
    }

    bool setPointLightProp(std::string prop, std::string val, PointLight& light) {
        if (setLightProp(prop, val, light)) return true;
        if (prop == "pos") {
            Vec3f pos;
            if (!fromString(val, pos)) return false;
            light.setPos(pos);
            return true;
        }
        if (prop == "falloff") {
            float falloff;
            if (!fromString(val, falloff)) return false;
            if (falloff < 0.0f) return false;
            light.setFalloff(falloff);
            return true;
        }
        return false;
    }

    bool setSpotLightProp(std::string prop, std::string val, SpotLight& light) {
        if (setPointLightProp(prop, val, light)) return true;
        if (prop == "dir") {
            Vec3f dir;
            if (!fromString(val, dir)) return false;
            if (!dir) return false;
            light.setDir(dir);
            return true;
        }
        if (prop == "width") {
            float width;
            if (!fromString(val, width)) return false;
            if (width < 0.0f || width > 1.0f) return false;
            light.setWidth(width);
            return true;
        }
        if (prop == "falloffexp") {
            float falloffExp;
            if (!fromString(val, falloffExp)) return false;
            light.setFalloffExp(falloffExp);
            return true;
        }
        return false;
    }

}

namespace ramiel {

    std::shared_ptr<Mesh> EngineMesh::get() {
        return mesh;
    }

    std::string EngineMesh::getProperty(std::string property) const {
        if (property == "vtxcount") return toString(mesh->getVertexCount());
        if (property == "tricount") return toString(mesh->getTriangleCount());
        return "";
    }

    void EngineMesh::setProperty(std::string property, std::string value) {}


    std::shared_ptr<Texture> EngineTexture::get() {
        return texture;
    }

    std::string EngineTexture::getProperty(std::string property) const {
        if (property == "size") return toString(texture->getSize());
        return "";
    }

    void EngineTexture::setProperty(std::string property, std::string value) {}


    void EngineGraphicsEntity::ctor() {
        addTask([this]() { addGraphicsEntity(&e); });
    }

    EngineGraphicsEntity::~EngineGraphicsEntity() {
        removeGraphicsEntity(&e);
    }

    Entity& EngineGraphicsEntity::get() {
        return e;
    }

    std::string EngineGraphicsEntity::getProperty(std::string property) const {
        return "entity property";
    }

    void EngineGraphicsEntity::setProperty(std::string property, std::string value) {

    }


    void EngineDirectionalLight::ctor() {
        addTask([this]() { addLight(&light); });
    }

    EngineDirectionalLight::~EngineDirectionalLight() {
        removeLight(&light);
    }

    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }

    std::string EngineDirectionalLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "dir") return toString(light.getDir());

        return "";
    }

    void EngineDirectionalLight::setProperty(std::string property, std::string value) {
        setDirLightProp(property, value, light);
    }


    void EnginePointLight::ctor() {
        addTask([this]() { addLight(&light); });
    }

    EnginePointLight::~EnginePointLight() {
        removeLight(&light);
    }

    PointLight& EnginePointLight::get() {
        return light;
    }

    std::string EnginePointLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "pos") return toString(light.getPos());
        if (property == "falloff") return toString(light.getFalloff());

        return "";
    }

    void EnginePointLight::setProperty(std::string property, std::string value) {
        setPointLightProp(property, value, light);
    }


    void EngineSpotLight::ctor() {
        addTask([this]() { addLight(&light); });
    }

    EngineSpotLight::~EngineSpotLight() {
        removeLight(&light);
    }

    SpotLight& EngineSpotLight::get() {
        return light;
    }

    std::string EngineSpotLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "pos") return toString(light.getPos());
        if (property == "falloff") return toString(light.getFalloff());

        if (property == "dir") return toString(light.getDir());
        if (property == "width") return toString(light.getWidth());
        if (property == "falloffexp") return toString(light.getFalloffExp());

        return "";
    }

    void EngineSpotLight::setProperty(std::string property, std::string value) {
        setSpotLightProp(property, value, light);
    }

}
