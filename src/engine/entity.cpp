#include "entity.h"
#include "graphics.h"
#include "task.h"

namespace ramiel {

    Mesh& EngineMesh::get() {
        return *mesh.get();
    }


    Texture& EngineTexture::get() {
        return *texture.get();
    }


    void EngineGraphicsEntity::ctor() {
        addTask([this]() { addGraphicsEntity(&e); });
    }

    EngineGraphicsEntity::~EngineGraphicsEntity() {
        addTask([this]() { removeGraphicsEntity(&e); });
    }

    Entity& EngineGraphicsEntity::get() {
        return e;
    }


    void EngineDirectionalLight::ctor() {
        addTask([this] () { addLight(&light); });
    }

    EngineDirectionalLight::~EngineDirectionalLight() {
        addTask([this]() { removeLight(&light); });
    }

    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }


    void EnginePointLight::ctor() {
        addTask([this] () { addLight(&light); });
    }

    EnginePointLight::~EnginePointLight() {
        addTask([this]() { removeLight(&light); });
    }

    PointLight& EnginePointLight::get() {
        return light;
    }


    void EngineSpotLight::ctor() {
        addTask([this] () { addLight(&light); });
    }

    EngineSpotLight::~EngineSpotLight() {
        addTask([this]() { removeLight(&light); });
    }

    SpotLight& EngineSpotLight::get() {
        return light;
    }

}
