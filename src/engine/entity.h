#pragma once

#include <string>
#include <memory>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

namespace ramiel {

    class EngineEntity {
    public:
        virtual ~EngineEntity() {}
    private:
        friend void add(std::string name, std::unique_ptr<EngineEntity>&& e);
        friend bool rm(std::string name);
        virtual void add() {}
        virtual void remove() {}
    };


    class EngineMesh : public EngineEntity {
    public:
        template<class... Ts>
        EngineMesh(Ts&&... args)
            : mesh(std::make_shared<Mesh>(std::forward<Ts>(args)...))
        {}
        std::shared_ptr<Mesh> get();
    private:
        // in the tree we have unique_ptr to shared_ptr to mesh,
        // not ideal but works with the graphics entity api
        std::shared_ptr<Mesh> mesh;
    };


    class EngineTexture : public EngineEntity {
    public:
        template<class... Ts>
        EngineTexture(Ts&&... args)
            : texture(std::make_shared<Texture>(std::forward<Ts>(args)...))
        {}
        std::shared_ptr<Texture> get();
    private:
        // see above comment
        std::shared_ptr<Texture> texture;
    };


    class EngineVertexShaderBase {
    private:
        friend class EngineGraphicsEntity;
        friend class EngineVertexShader;
        friend class EngineVertexShaderTextured;
        virtual std::unique_ptr<VertexShaderBase> make() const = 0;
    };

    class EngineVertexShader : public EngineVertexShaderBase {
    public:
        EngineVertexShader(Mat4x4f worldTransform);
        virtual std::unique_ptr<VertexShaderBase> make() const override;
    private:
        Mat4x4f worldTransform;
    };

    class EngineVertexShaderTextured : public EngineVertexShaderBase {
    public:
        EngineVertexShaderTextured(Mat4x4f worldTransform);
        virtual std::unique_ptr<VertexShaderBase> make() const override;
    private:
        Mat4x4f worldTransform;
    };


    class EnginePixelShaderBase {
    private:
        friend class EngineGraphicsEntity;
        friend class EnginePixelShader;
        friend class EnginePixelShaderTextured;
        virtual std::unique_ptr<PixelShaderBase> make() const = 0;
    };

    class EnginePixelShader : public EnginePixelShaderBase {
    public:
        EnginePixelShader(
            Vec3f surfaceColor,
            float specularExponent,
            float specularIntensity
        );
        virtual std::unique_ptr<PixelShaderBase> make() const override;
    private:
        Vec3f surfaceColor;
        float specularExponent;
        float specularIntensity;
    };

    class EnginePixelShaderTextured : public EnginePixelShaderBase {
    public:
        EnginePixelShaderTextured(
            std::string texturePath,
            float specularExponent,
            float specularIntensity
        );
        virtual std::unique_ptr<PixelShaderBase> make() const override;
    private:
        std::shared_ptr<Texture> texture;
        float specularExponent;
        float specularIntensity;
    };


    class EngineGraphicsEntity : public EngineEntity {
    public:
        EngineGraphicsEntity(
            std::string meshPath,
            const std::unique_ptr<EngineVertexShaderBase>& vs,
            const std::unique_ptr<EnginePixelShaderBase>& ps
        );
        Entity& get();
    private:
        virtual void add() override;
        virtual void remove() override;
        Entity e;
    };


    class EngineDirectionalLight : public EngineEntity {
    public:
        template<class... Ts>
        EngineDirectionalLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        {}
        DirectionalLight& get();
    private:
        virtual void add() override;
        virtual void remove() override;
        DirectionalLight light;
    };


    class EnginePointLight : public EngineEntity {
    public:
        template<class... Ts>
        EnginePointLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        {}
        PointLight& get();
    private:
        virtual void add() override;
        virtual void remove() override;
        PointLight light;
    };


    class EngineSpotLight : public EngineEntity {
    public:
        template<class... Ts>
        EngineSpotLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        {}
        SpotLight& get();
    private:
        virtual void add() override;
        virtual void remove() override;
        SpotLight light;
    };

}
