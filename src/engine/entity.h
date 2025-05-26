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


    class EnginePhysicsEntity : public EngineEntity {
    public:
        using H = std::shared_ptr<EnginePhysicsEntity>;

        virtual Mat4x4f getTransform() const = 0;

    protected:
        EnginePhysicsEntity(const std::string& name)
            : EngineEntity(name)
        {}
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
        void updatePhys();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        EngineGraphicsEntity(
            const std::string& name,
            EngineMesh::H mesh,
            EnginePhysicsEntity::H phys,
            std::unique_ptr<EngineVertexShaderBase>&& vs,
            std::unique_ptr<EnginePixelShaderBase>&& ps
        )
            : EngineEntity(name)
            , mesh(mesh)
            , phys(phys)
            , vs(std::move(vs))
            , ps(std::move(ps))
            , e(&mesh->get(), vs->get(), ps->get())
        {}

        Entity e;

        EnginePhysicsEntity::H phys;
        EngineMesh::H mesh;

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


    class EngineStaticPhysics : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineStaticPhysics>;

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        EngineStaticPhysics(const std::string& name, Vec3f pos, Vec3f rot)
            : EnginePhysicsEntity(name)
            , pos(pos)
            , rot(rot)
        {}

        Vec3f pos;
        Vec3f rot;
    };


    class EngineParticle : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineParticle>;

        Particle& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineParticle(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        Particle e;
    };


    class EngineRigidBody : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineRigidBody>;

        RigidBody& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineRigidBody(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        RigidBody e;
    };


    class EngineParticleCollider : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineParticleCollider>;

        ParticleCollider& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineParticleCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        ParticleCollider e;
    };


    class EnginePlaneCollider : public EngineEntity {
    public:
        using H = std::shared_ptr<EnginePlaneCollider>;

        PlaneCollider& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EnginePlaneCollider(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        PlaneCollider e;
    };


    class EngineSphereCollider : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineSphereCollider>;

        SphereCollider& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineSphereCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        SphereCollider e;
    };


    class EngineBoxCollider : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineBoxCollider>;

        BoxCollider& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineBoxCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        BoxCollider e;
    };


    class EngineDistanceConstraint : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineDistanceConstraint>;

        DistanceConstraint& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineDistanceConstraint(
            const std::string& name,
            bool visible,
            Ts&&... args
        )
            : EngineEntity(name)
            , visible(visible)
            , c(std::forward<Ts>(args)...)
        {}

        bool visible;
        DistanceConstraint c;
    };


    class EngineRopeConstraint : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineRopeConstraint>;

        RopeConstraint& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineRopeConstraint(
            const std::string& name,
            bool visible,
            Ts&&... args
        )
            : EngineEntity(name)
            , visible(visible)
            , c(std::forward<Ts>(args)...)
        {}

        bool visible;
        RopeConstraint c;
    };

}
