#pragma once

#include <string>
#include <memory>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

namespace ramiel {

    class EngineMesh {
    public:
        template<class... Ts>
        EngineMesh(Ts&&... args)
            : mesh(std::make_shared<Mesh>(std::forward<Ts>(args)...))
        {}
        Mesh& get();
    private:
        std::shared_ptr<Mesh> mesh; // using shared_ptr for graphics api
    };


    class EngineTexture {
    public:
        template<class... Ts>
        EngineTexture(Ts&&... args)
            : texture(std::make_shared<Texture>(std::forward<Ts>(args)...))
        {}
        Texture& get();
    private:
        std::shared_ptr<Texture> texture; // using shared_ptr for graphics api
    };


    class EngineGraphicsEntity {
    public:
        template<class... Ts>
        EngineGraphicsEntity(Ts&&... args)
            : e(std::forward<Ts>(args)...)
        { ctor(); }
        ~EngineGraphicsEntity();
        Entity& get();
    private:
        void ctor();
        Entity e;
    };


    class EngineDirectionalLight {
    public:
        template<class... Ts>
        EngineDirectionalLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        { ctor(); }
        ~EngineDirectionalLight();
        DirectionalLight& get();
    private:
        void ctor();
        DirectionalLight light;
    };


    class EnginePointLight {
    public:
        template<class... Ts>
        EnginePointLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        { ctor(); }
        ~EnginePointLight();
        PointLight& get();
    private:
        void ctor();
        PointLight light;
    };


    class EngineSpotLight {
    public:
        template<class... Ts>
        EngineSpotLight(Ts&&... args)
            : light(std::forward<Ts>(args)...)
        { ctor(); }
        ~EngineSpotLight();
        SpotLight& get();
    private:
        void ctor();
        SpotLight light;
    };

}
