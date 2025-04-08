#include "entity.h"
#include "graphics.h"
#include "task.h"

namespace ramiel {

    std::shared_ptr<Mesh> EngineMesh::get() {
        return mesh;
    }

    std::string EngineMesh::getProperty(std::string property) const {
        return "mesh property";
    }

    void EngineMesh::setProperty(std::string property, std::string value) {

    }


    std::shared_ptr<Texture> EngineTexture::get() {
        return texture;
    }

    std::string EngineTexture::getProperty(std::string property) const {
        return "texture property";
    }

    void EngineTexture::setProperty(std::string property, std::string value) {

    }


    void EngineGraphicsEntity::ctor() {
        addTask([this]() { addGraphicsEntity(&e); });
    }

    EngineGraphicsEntity::~EngineGraphicsEntity() {
        addTask([e = &e]() { removeGraphicsEntity(e); });
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
        addTask([this]() { removeLight(&light); });
    }

    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }

    std::string EngineDirectionalLight::getProperty(std::string property) const {
        return "dir light property";
    }

    void EngineDirectionalLight::setProperty(std::string property, std::string value) {

    }


    void EnginePointLight::ctor() {
        addTask([this]() { addLight(&light); });
    }

    EnginePointLight::~EnginePointLight() {
        addTask([this]() { removeLight(&light); });
    }

    PointLight& EnginePointLight::get() {
        return light;
    }

    std::string EnginePointLight::getProperty(std::string property) const {
        return "point light property";
    }

    void EnginePointLight::setProperty(std::string property, std::string value) {

    }


    void EngineSpotLight::ctor() {
        addTask([this]() { addLight(&light); });
    }

    EngineSpotLight::~EngineSpotLight() {
        addTask([this]() { removeLight(&light); });
    }

    SpotLight& EngineSpotLight::get() {
        return light;
    }

    std::string EngineSpotLight::getProperty(std::string property) const {
        return "spot light property";
    }

    void EngineSpotLight::setProperty(std::string property, std::string value) {

    }

}
