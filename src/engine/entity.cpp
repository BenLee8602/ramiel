#include "entity.h"
#include "graphicsprivate.h"
#include "engine.h"

namespace ramiel {

    std::shared_ptr<Mesh> EngineMesh::get() {
        return mesh;
    }


    std::shared_ptr<Texture> EngineTexture::get() {
        return texture;
    }


    EngineVertexShader::EngineVertexShader(Mat4x4f worldTransform)
        : worldTransform(worldTransform)
    {}

    std::unique_ptr<VertexShaderBase> EngineVertexShader::make() const {
        return std::make_unique<VertexShader>(worldTransform);
    }


    EngineVertexShaderTextured::EngineVertexShaderTextured(Mat4x4f worldTransform)
        : worldTransform(worldTransform)
    {}

    std::unique_ptr<VertexShaderBase> EngineVertexShaderTextured::make() const {
        return std::make_unique<VertexShaderTextured>(worldTransform);
    }


    EnginePixelShader::EnginePixelShader(
        Vec3f surfaceColor,
        float specularExponent,
        float specularIntensity
    )
        : surfaceColor(surfaceColor)
        , specularExponent(specularExponent)
        , specularIntensity(specularIntensity)
    {}

    std::unique_ptr<PixelShaderBase> EnginePixelShader::make() const {
        return std::make_unique<PixelShader>(
            surfaceColor,
            specularExponent,
            specularIntensity,
            Vec3f{}
        );
    }


    EnginePixelShaderTextured::EnginePixelShaderTextured(
        std::string texturePath,
        float specularExponent,
        float specularIntensity
    )
        : texture(nullptr)
        , specularExponent(specularExponent)
        , specularIntensity(specularIntensity)
    {
        auto node = dynamic_cast<EngineTexture*>(get(texturePath));
        if (node) texture = node->get();
    }

    std::unique_ptr<PixelShaderBase> EnginePixelShaderTextured::make() const {
        if (!texture) return nullptr;
        return std::make_unique<PixelShaderTextured>(
            texture,
            specularExponent,
            specularIntensity,
            Vec3f{}
        );
    }


    EngineGraphicsEntity::EngineGraphicsEntity(
        std::string meshPath,
        const std::unique_ptr<EngineVertexShaderBase>& vsf,
        const std::unique_ptr<EnginePixelShaderBase>& psf
    ) : e(Entity()) {
        auto meshNode = dynamic_cast<EngineMesh*>(ramiel::get(meshPath));
        e = Entity(
            meshNode ? meshNode->get() : nullptr,
            vsf->make(),
            psf->make()
        );
    }

    Entity& EngineGraphicsEntity::get() {
        return e;
    }

    void EngineGraphicsEntity::add() {
        addGraphicsEntity(&e);
    }

    void EngineGraphicsEntity::remove() {
        removeGraphicsEntity(&e);
    }


    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }

    void EngineDirectionalLight::add() {
        addLight(&light);
    }

    void EngineDirectionalLight::remove() {
        removeLight(&light);
    }


    PointLight& EnginePointLight::get() {
        return light;
    }

    void EnginePointLight::add() {
        addLight(&light);
    }

    void EnginePointLight::remove() {
        removeLight(&light);
    }


    SpotLight& EngineSpotLight::get() {
        return light;
    }

    void EngineSpotLight::add() {
        addLight(&light);
    }

    void EngineSpotLight::remove() {
        removeLight(&light);
    }

}
