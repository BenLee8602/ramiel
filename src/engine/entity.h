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
        static typename T::H make(Ts&&... args) {
            static_assert(std::is_base_of_v<EngineEntity, T>);
            return typename T::H(new T(std::forward<Ts>(args)...));
        }

        template<class T>
        static typename T::H cast(Tree::H t) {
            static_assert(std::is_base_of_v<EngineEntity, T>);
            return std::dynamic_pointer_cast<T>(t);
        }

        static void enableAll(Tree::H e);
        static void disableAll(Tree::H e);

        static EngineEntity::H copyAll(Tree::H e);

        static void serializeAll(BinaryWriter& file, Tree::H t);
        static EngineEntity::H deserializeAll(BinaryReader& file);

        static void addRef(std::string path, std::function<void(Tree::H)> fn);

        virtual std::string getProperty(std::string property) const = 0;
        virtual void setProperty(std::string property, std::string value) = 0;

        virtual void enable() {}
        virtual void disable() {}

        virtual EngineEntity::H copy() const = 0;
        virtual void serialize(BinaryWriter& file) const = 0;

    protected:
        EngineEntity(const std::string& name) : Tree(name) {}
    };


    class EnginePhysicsEntity : public EngineEntity {
    public:
        using H = std::shared_ptr<EnginePhysicsEntity>;

        virtual Mat4x4f getTransform() const = 0;
        virtual Particle* getParticle();
        virtual RigidBody* getRigidBody();

    protected:
        EnginePhysicsEntity(const std::string& name)
            : EngineEntity(name)
        {}
    };


    class EngineDir : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineDir>;

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        EngineDir(const std::string& name)
            : EngineEntity(name)
        {}

        EngineDir(BinaryReader& file);
    };


    class EngineMesh : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineMesh>;

        Mesh& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

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

        EngineMesh(BinaryReader& file);

        std::shared_ptr<Mesh> mesh;
    };


    class EngineTexture : public EngineEntity {
    public:
        using H = std::shared_ptr<EngineTexture>;

        Texture& get();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

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

        EngineTexture(BinaryReader& file);

        std::shared_ptr<Texture> texture;
    };


    class EngineGraphicsEntity : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineGraphicsEntity>;

        Entity& get();
        void updatePhys();

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual void enable() override;
        virtual void disable() override;

        virtual EngineEntity::H copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;
        virtual RigidBody* getRigidBody() override;

    private:
        friend EngineEntity;

        EngineGraphicsEntity(const std::string& name)
            : EnginePhysicsEntity(name)
            , mesh(nullptr)
            , phys(nullptr)
            , vs(nullptr)
            , ps(nullptr)
            , e()
        {}

        EngineGraphicsEntity(
            const std::string& name,
            EngineMesh::H mesh,
            EnginePhysicsEntity::H phys,
            std::unique_ptr<EngineVertexShaderBase>&& vs,
            std::unique_ptr<EnginePixelShaderBase>&& ps
        )
            : EnginePhysicsEntity(name)
            , mesh(mesh)
            , phys(phys)
            , vs(std::move(vs))
            , ps(std::move(ps))
            , e(&mesh->get(), vs->get(), ps->get())
        {}

        EngineGraphicsEntity(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineDirectionalLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        EngineDirectionalLight(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EnginePointLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        EnginePointLight(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineSpotLight(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , light(std::forward<Ts>(args)...)
        {}

        EngineSpotLight(BinaryReader& file);

        SpotLight light;
    };


    class EngineStaticPhysics : public EnginePhysicsEntity {
    public:
        using H = std::shared_ptr<EngineStaticPhysics>;

        virtual std::string getProperty(std::string property) const override;
        virtual void setProperty(std::string property, std::string value) override;

        virtual EngineEntity::H copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;

    private:
        friend EngineEntity;

        EngineStaticPhysics(const std::string& name, Vec3f pos, Vec3f rot)
            : EnginePhysicsEntity(name)
            , pos(pos)
            , rot(rot)
        {}

        EngineStaticPhysics(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineParticle(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EngineParticle(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;
        virtual RigidBody* getRigidBody() override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineRigidBody(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EngineRigidBody(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineParticleCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EngineParticleCollider(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EnginePlaneCollider(const std::string& name, Ts&&... args)
            : EngineEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EnginePlaneCollider(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;
        virtual RigidBody* getRigidBody() override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineSphereCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EngineSphereCollider(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

        virtual Mat4x4f getTransform() const override;
        virtual Particle* getParticle() override;
        virtual RigidBody* getRigidBody() override;

    private:
        friend EngineEntity;

        template<class... Ts>
        EngineBoxCollider(const std::string& name, Ts&&... args)
            : EnginePhysicsEntity(name)
            , e(std::forward<Ts>(args)...)
        {}

        EngineBoxCollider(BinaryReader& file);

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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        EngineDistanceConstraint(
            const std::string& name,
            bool visible,
            float l0,
            float a,
            EnginePhysicsEntity::H e1,
            EnginePhysicsEntity::H e2
        )
            : EngineEntity(name)
            , visible(visible)
            , c(l0, a, e1->getParticle(), e2->getParticle())
            , e1(e1)
            , e2(e2)
        {}

        EngineDistanceConstraint(BinaryReader& file);

        bool visible;
        DistanceConstraint c;
        EnginePhysicsEntity::H e1;
        EnginePhysicsEntity::H e2;
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
        virtual void serialize(BinaryWriter& file) const override;

    private:
        friend EngineEntity;

        EngineRopeConstraint(
            const std::string& name,
            bool visible,
            float l0,
            float a,
            EnginePhysicsEntity::H e1,
            EnginePhysicsEntity::H e2,
            Vec3f r1,
            Vec3f r2
        )
            : EngineEntity(name)
            , visible(visible)
            , c(l0, a, e1->getRigidBody(), r1, e2->getRigidBody(), r2)
            , e1(e1)
            , e2(e2)
        {}

        EngineRopeConstraint(BinaryReader& file);

        bool visible;
        RopeConstraint c;
        EnginePhysicsEntity::H e1;
        EnginePhysicsEntity::H e2;
    };

}
