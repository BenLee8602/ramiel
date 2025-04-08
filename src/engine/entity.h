#pragma once

#include <string>
#include <memory>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

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

        virtual std::string getProperty(std::string property) const = 0;
        virtual void setProperty(std::string property, std::string value) = 0;

    protected:
        EngineEntity(const std::string& name) : Tree(name) {}
    };


    class EngineMesh : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineMesh>;

        std::shared_ptr<Mesh> get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineMesh(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , mesh(std::make_shared<Mesh>(std::forward<Ts>(args)...))
        {}

        std::shared_ptr<Mesh> mesh; // using shared_ptr for graphics api
    };


    class EngineTexture : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineTexture>;

        std::shared_ptr<Texture> get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineTexture(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , texture(std::make_shared<Texture>(std::forward<Ts>(args)...))
        {}

        std::shared_ptr<Texture> texture; // using shared_ptr for graphics api
    };


    class EngineGraphicsEntity : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineGraphicsEntity>;

        ~EngineGraphicsEntity();
        Entity& get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineGraphicsEntity(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , e(std::forward<Ts>(args)...)
        { ctor(); }
        void ctor();

        Entity e;
    };


    class EngineDirectionalLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineDirectionalLight>;

        ~EngineDirectionalLight();
        DirectionalLight& get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineDirectionalLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        { ctor(); }
        void ctor();

        DirectionalLight light;
    };


    class EnginePointLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EnginePointLight>;

        ~EnginePointLight();
        PointLight& get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EnginePointLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        { ctor(); }
        void ctor();

        PointLight light;
    };


    class EngineSpotLight : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineSpotLight>;

        ~EngineSpotLight();
        SpotLight& get();
        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineSpotLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        { ctor(); }
        void ctor();

        SpotLight light;
    };

}
