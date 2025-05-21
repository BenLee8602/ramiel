#pragma once

#include <string>
#include <memory>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>
#include "shader.h"

namespace ramiel {

    class EngineEntity : public Tree {
    public:
        using H = std::shared_ptr<EngineEntity>;

        template<class T, class... Ts>
        static typename T::H make(const std::string& name, Ts&&... args) {
            static_assert(std::is_base_of_v<EngineEntity, T>);
            return typename T::H(
                new T(name, std::forward<Ts>(args)...)
            );
        }

        template<class T>
        static typename T::H cast(Tree::H t) {
            static_assert(std::is_base_of_v<EngineEntity, T>);
            return std::dynamic_pointer_cast<T>(t);
        }

        static bool enableAll(Tree::H e);
        static bool disableAll(Tree::H e);

        static Tree::H copyAll(Tree::H e);

        virtual std::string getProperty(std::string property) const = 0;
        virtual void setProperty(std::string property, std::string value) = 0;

        virtual void enable() {}
        virtual void disable() {}

        virtual EngineEntity::H copy() const = 0;

    protected:
        EngineEntity(const std::string& name) : Tree(name) {}
    };


    class EngineMesh : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineMesh>;

        Mesh& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineMesh(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , mesh(std::make_shared<Mesh>(std::forward<Ts>(args)...))
        {}

        EngineMesh(const std::string& name, std::shared_ptr<Mesh> mesh)
            : EngineEntity(name)
            , mesh(mesh)
        {}

        std::shared_ptr<Mesh> mesh;
    };


    class EngineTexture : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineTexture>;

        Texture& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineTexture(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , texture(std::make_shared<Texture>(std::forward<Ts>(args)...))
        {}

        EngineTexture(const std::string& name, std::shared_ptr<Texture> texture)
            : EngineEntity(name)
            , texture(texture)
        {}

        std::shared_ptr<Texture> texture;
    };


    class EngineGraphicsEntity : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineGraphicsEntity>;

        Entity& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        EngineGraphicsEntity(
            const std::string& name,
            const std::string& mesh,
            std::unique_ptr<EngineVertexShaderBase>&& vs,
            std::unique_ptr<EnginePixelShaderBase>&& ps
        );

        Entity e;
        std::string mesh;
        std::unique_ptr<EngineVertexShaderBase> vs;
        std::unique_ptr<EnginePixelShaderBase> ps;
    };


    class EngineDirectionalLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineDirectionalLight>;

        DirectionalLight& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineDirectionalLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        DirectionalLight light;
    };


    class EnginePointLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EnginePointLight>;

        PointLight& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EnginePointLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        PointLight light;
    };


    class EngineSpotLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineSpotLight>;

        SpotLight& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineSpotLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        SpotLight light;
    };

}
