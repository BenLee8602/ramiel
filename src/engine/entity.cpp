#include "entity.h"
#include "graphics.h"

namespace ramiel {

    Mesh& EngineMesh::get() {
        return *mesh.get();
    }


    Texture& EngineTexture::get() {
        return *texture.get();
    }


    void EngineGraphicsEntity::ctor() {
        addGraphicsEntity(&e);
    }

    EngineGraphicsEntity::~EngineGraphicsEntity() {
        removeGraphicsEntity(&e);
    }

    Entity& EngineGraphicsEntity::get() {
        return e;
    }


    void EngineDirectionalLight::ctor() {
        addGraphicsLight(&light);
    }

    EngineDirectionalLight::~EngineDirectionalLight() {
        removeLight(&light);
    }

    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }


    void EnginePointLight::ctor() {
        addGraphicsLight(&light);
    }

    EnginePointLight::~EnginePointLight() {
        removeLight(&light);
    }

    PointLight& EnginePointLight::get() {
        return light;
    }


    void EngineSpotLight::ctor() {
        addGraphicsLight(&light);
    }

    EngineSpotLight::~EngineSpotLight() {
        removeLight(&light);
    }

    SpotLight& EngineSpotLight::get() {
        return light;
    }

}
