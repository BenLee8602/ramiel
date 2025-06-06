#include <cassert>
#include <cstring>

#include "entity.h"
#include "graphics.h"
#include "serialize.h"
#include "command.h"
#include "task.h"
using namespace ramiel;

namespace {

    struct TreeRef {
        std::string path;
        std::function<void(Tree::H)> fn;
        TreeRef(
            std::string path,
            std::function<void(Tree::H)> fn
        )
            : path(path)
            , fn(fn)
        {}
    };
    std::vector<TreeRef> refs;

    void resolveRefs(Tree::H root) {
        for (auto& ref : refs) {
            ref.fn(root->getRelative(ref.path));
        }
        refs = std::vector<TreeRef>();
    }


    EngineEntity::H copyAllNodes(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        assert(ee);
        EngineEntity::H out = ee->copy();
        e->forEachKid([out](Tree::H e) {
            auto e2 = copyAllNodes(e);
            assert(e2);
            out->insert(e2);
            return true;
        });
        return out;
    }


    EngineEntity::H deserializeAllNodes(BinaryReader& file) {
        file.next();

        std::string type = readString(file);
        EngineEntity::H e;

        if (type == "dir")
            e = EngineEntity::make<EngineDir>(file);
        else if (type == "mesh")
            e = EngineEntity::make<EngineMesh>(file);
        else if (type == "texture")
            e = EngineEntity::make<EngineTexture>(file);
        else if (type == "entity")
            e = EngineEntity::make<EngineGraphicsEntity>(file);
        else if (type == "dirlight")
            e = EngineEntity::make<EngineDirectionalLight>(file);
        else if (type == "pointlight")
            e = EngineEntity::make<EnginePointLight>(file);
        else if (type == "spotlight")
            e = EngineEntity::make<EngineSpotLight>(file);
        else if (type == "staticphys")
            e = EngineEntity::make<EngineStaticPhysics>(file);
        else if (type == "particle")
            e = EngineEntity::make<EngineParticle>(file);
        else if (type == "rigidbody")
            e = EngineEntity::make<EngineRigidBody>(file);
        else if (type == "particlecollider")
            e = EngineEntity::make<EngineParticleCollider>(file);
        else if (type == "planecollider")
            e = EngineEntity::make<EnginePlaneCollider>(file);
        else if (type == "spherecollider")
            e = EngineEntity::make<EngineSphereCollider>(file);
        else if (type == "boxcollider")
            e = EngineEntity::make<EngineBoxCollider>(file);
        else if (type == "distanceconstraint")
            e = EngineEntity::make<EngineDistanceConstraint>(file);
        else if (type == "ropeconstraint")
            e = EngineEntity::make<EngineRopeConstraint>(file);

        assert(e);
        while (file.mode() == BinaryReader::Mode::NODE_START) {
            e->insert(deserializeAllNodes(file));
        }

        file.next();
        return e;
    }


    bool setLightProp(std::string prop, std::string val, Light& light) {
        if (prop == "color") {
            Vec3f color;
            if (!fromString(val, color)) return false;
            if (color < 0.0f) return false;
            light.setColor(color);
            return true;
        }
        if (prop == "intensity") {
            float intensity;
            if (!fromString(val, intensity)) return false;
            if (intensity < 0.0f) return false;
            light.setIntensity(intensity);
            return true;
        }
        return false;
    }

    bool setDirLightProp(std::string prop, std::string val, DirectionalLight& light) {
        if (setLightProp(prop, val, light)) return true;
        if (prop == "dir") {
            Vec3f dir;
            if (!fromString(val, dir)) return false;
            if (!dir) return false;
            light.setDir(dir);
            return true;
        }
        return false;
    }

    bool setPointLightProp(std::string prop, std::string val, PointLight& light) {
        if (setLightProp(prop, val, light)) return true;
        if (prop == "pos") {
            Vec3f pos;
            if (!fromString(val, pos)) return false;
            light.setPos(pos);
            return true;
        }
        if (prop == "falloff") {
            float falloff;
            if (!fromString(val, falloff)) return false;
            if (falloff < 0.0f) return false;
            light.setFalloff(falloff);
            return true;
        }
        return false;
    }

    bool setSpotLightProp(std::string prop, std::string val, SpotLight& light) {
        if (setPointLightProp(prop, val, light)) return true;
        if (prop == "dir") {
            Vec3f dir;
            if (!fromString(val, dir)) return false;
            if (!dir) return false;
            light.setDir(dir);
            return true;
        }
        if (prop == "width") {
            float width;
            if (!fromString(val, width)) return false;
            if (width < 0.0f || width > 1.0f) return false;
            light.setWidth(width);
            return true;
        }
        if (prop == "falloffexp") {
            float falloffExp;
            if (!fromString(val, falloffExp)) return false;
            light.setFalloffExp(falloffExp);
            return true;
        }
        return false;
    }


    std::string getParticleProp(std::string prop, const Particle& e) {
        if (prop == "pos") return toString(e.pos);
        if (prop == "vel") return toString(e.vel);
        if (prop == "mass") return toString(1.0f / e.wass);
        return "";
    }

    std::string getRigidBodyProp(std::string prop, const RigidBody& e) {
        std::string pres = getParticleProp(prop, e);
        if (!pres.empty()) return pres;
        if (prop == "rot") return toString(e.rot);
        if (prop == "rotaxis") return toString(e.rotAxis);
        return "";
    }

    bool setParticleProp(std::string prop, std::string val, Particle& e) {
        if (prop == "pos") fromString(val, e.pos);
        else if (prop == "vel") fromString(val, e.vel);
        else if (prop == "mass") {
            float mass;
            if (!fromString(val, mass)) return false;
            if (mass <= 0.0f) return false;
            e.wass = 1.0f / mass;
            return true;
        }
        return false;
    }

    bool setRigidBodyProp(std::string prop, std::string val, RigidBody& e) {
        if (setParticleProp(prop, val, e)) return true;
        if (prop == "rot") fromString(val, e.rot);
        else if (prop == "rotaxis") fromString(val, e.rotAxis);
        return false;
    }

}

namespace ramiel {

    void EngineEntity::enableAll(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        assert(ee);
        ee->enable();
        e->forEachKid([](Tree::H e) {
            enableAll(e);
            return true;
        });
    }

    void EngineEntity::disableAll(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        assert(ee);
        ee->disable();
        e->forEachKid([](Tree::H e) {
            disableAll(e);
            return true;
        });
    }


    EngineEntity::H EngineEntity::copyAll(Tree::H e) {
        EngineEntity::H root = copyAllNodes(e);
        resolveRefs(root);
        return root;
    }


    void EngineEntity::serializeAll(BinaryWriter& file, Tree::H t) {
        file.startNode();

        auto ee = EngineEntity::cast<EngineEntity>(t);
        ee->serialize(file);

        ee->forEachKid([&file](Tree::H t) {
            serializeAll(file, t);
            return true;
        });

        file.endNode();
    }

    EngineEntity::H EngineEntity::deserializeAll(BinaryReader& file) {
        EngineEntity::H root = deserializeAllNodes(file);
        resolveRefs(root);
        return root;
    }

    void EngineEntity::addRef(
        std::string path,
        std::function<void(Tree::H)> fn
    ) {
        refs.emplace_back(path, fn);
    }


    Particle* EnginePhysicsEntity::getParticle() {
        return nullptr;
    }

    RigidBody* EnginePhysicsEntity::getRigidBody() {
        return nullptr;
    }


    std::string EngineDir::getProperty(std::string property) const {
        return "";
    }

    void EngineDir::setProperty(std::string property, std::string value) {}

    EngineEntity::H EngineDir::copy() const {
        return EngineEntity::make<EngineDir>(getName());
    }

    void EngineDir::serialize(BinaryWriter& file) const {
        writeString(file, "dir");
        writeString(file, getName());
    }

    EngineDir::EngineDir(BinaryReader& file)
        : EngineEntity(readString(file))
    {}


    Mesh& EngineMesh::get() {
        return *mesh;
    }

    std::string EngineMesh::getProperty(std::string property) const {
        if (property == "vtxcount") return toString(mesh->getVertexCount());
        if (property == "tricount") return toString(mesh->getTriangleCount());
        return "";
    }

    void EngineMesh::setProperty(std::string property, std::string value) {}


    EngineEntity::H EngineMesh::copy() const {
        return EngineEntity::make<EngineMesh>(getName(), mesh);
    }

    void EngineMesh::serialize(BinaryWriter& file) const {
        writeString(file, "mesh");
        writeString(file, getName());

        file.writeAttr(
            mesh->vtxBegin(),
            mesh->getVertexCount() * mesh->getVertexSize() * sizeof(float)
        );
        file.writeAttr(
            mesh->triBegin(),
            mesh->getTriangleCount() * 3 * sizeof(uint32_t)
        );
        writeVector(file, mesh->getAttrOutType());
    }

    EngineMesh::EngineMesh(BinaryReader& file)
        : EngineEntity(readString(file))
    {
        auto v = readVector<float>(file);
        auto f = readVector<uint32_t>(file);
        auto attr = readVector<uint8_t>(file);
        mesh = std::make_shared<Mesh>(
            std::move(v), std::move(f), std::move(attr));
    }


    Texture& EngineTexture::get() {
        return *texture;
    }

    std::string EngineTexture::getProperty(std::string property) const {
        if (property == "size") return toString(texture->getSize());
        return "";
    }

    void EngineTexture::setProperty(std::string property, std::string value) {}

    EngineEntity::H EngineTexture::copy() const {
        return EngineEntity::make<EngineTexture>(getName(), texture);
    }

    void EngineTexture::serialize(BinaryWriter& file) const {
        writeString(file, "texture");
        writeString(file, getName());

        Vec2u size = texture->getSize();
        writeValue(file, size);

        std::vector<uint8_t> dataOut(size[X] * size[Y] * 3);
        float* dataIn = reinterpret_cast<float*>(texture->getData());
        for (size_t i = 0; i < dataOut.size(); i++) {
            dataOut[i] = static_cast<uint8_t>(dataIn[i] * 255.0f);
        }
        writeVector(file, dataOut);
    }

    EngineTexture::EngineTexture(BinaryReader& file)
        : EngineEntity(readString(file))
    {
        Vec2u size = readValue<Vec2u>(file);

        std::vector<Vec3f> data(size[X] * size[Y]);
        auto dataIn = readVector<uint8_t>(file);
        float* dataOut = reinterpret_cast<float*>(data.data());
        for (size_t i = 0; i < dataIn.size(); i++) {
            dataOut[i] = static_cast<float>(dataIn[i]) / 255.0f;
        }

        texture = std::make_shared<Texture>(std::move(data), size);
    }


    Entity& EngineGraphicsEntity::get() {
        return e;
    }

    void EngineGraphicsEntity::updatePhys() {
        vs->setTransform(phys->getTransform());
    }

    std::string EngineGraphicsEntity::getProperty(std::string prop) const {
        std::string val;
        if (prop == "mesh") return mesh->getPath();
        val = vs->getProp(prop);
        if (!val.empty()) return val;
        val = ps->getProp(prop);
        if (!val.empty()) return val;
        val = phys->getProperty(prop);
        if (!val.empty()) return val;
        return "";
    }

    void EngineGraphicsEntity::setProperty(std::string prop, std::string val) {
        vs->setProp(prop, val);
        ps->setProp(prop, val);
        phys->setProperty(prop, val);
        renderNeeded();
    }

    void EngineGraphicsEntity::enable() {
        addGraphicsEntity(this);
        phys->enable();
        renderNeeded();
    }

    void EngineGraphicsEntity::disable() {
        removeGraphicsEntity(this);
        phys->disable();
        renderNeeded();
    }

    EngineEntity::H EngineGraphicsEntity::copy() const {
        auto e = EngineEntity::make<EngineGraphicsEntity>(getName());
        e->phys = EngineEntity::cast<EnginePhysicsEntity>(phys->copy());
        e->vs = std::unique_ptr<EngineVertexShaderBase>(vs->copy());
        e->ps = std::unique_ptr<EnginePixelShaderBase>(ps->copy());

        refs.emplace_back(mesh->getPath(), [=](Tree::H node) {
            e->setName(getName());
            e->mesh = EngineEntity::cast<EngineMesh>(node);
            e->e = Entity(&e->mesh->get(), e->vs->get(), e->ps->get());
        });
        return e;
    }

    void EngineGraphicsEntity::serialize(BinaryWriter& file) const {
        writeString(file, "entity");
        writeString(file, getName());

        writeString(file, mesh->getPath());

        file.startNode();
        phys->serialize(file);
        file.endNode();

        file.startNode();
        vs->serialize(file);
        file.endNode();

        file.startNode();
        ps->serialize(file);
        file.endNode();
    }

    Mat4x4f EngineGraphicsEntity::getTransform() const {
        assert(phys);
        return phys->getTransform();
    }

    Particle* EngineGraphicsEntity::getParticle() {
        assert(phys);
        return phys->getParticle();
    }

    RigidBody* EngineGraphicsEntity::getRigidBody() {
        assert(phys);
        return phys->getRigidBody();
    }

    EngineGraphicsEntity::EngineGraphicsEntity(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
    {
        std::string meshPath = readString(file);
        phys = cast<EnginePhysicsEntity>(deserializeAllNodes(file));
        vs = std::unique_ptr<EngineVertexShaderBase>(
            EngineVertexShaderBase::make(file));
        ps = std::unique_ptr<EnginePixelShaderBase>(
            EnginePixelShaderBase::make(file));

        refs.emplace_back(meshPath, [=](Tree::H node) {
            mesh = EngineEntity::cast<EngineMesh>(node);
            e = Entity(&mesh->get(), vs->get(), ps->get());
            assert(mesh && e);
        });
    }


    DirectionalLight& EngineDirectionalLight::get() {
        return light;
    }

    std::string EngineDirectionalLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "dir") return toString(light.getDir());

        return "";
    }

    void EngineDirectionalLight::setProperty(std::string property, std::string value) {
        setDirLightProp(property, value, light);
        renderNeeded();
    }

    void EngineDirectionalLight::enable() {
        addLight(&light);
        renderNeeded();
    }

    void EngineDirectionalLight::disable() {
        removeLight(&light);
        renderNeeded();
    }

    EngineEntity::H EngineDirectionalLight::copy() const {
        return EngineEntity::make<EngineDirectionalLight>(getName(), light);
    }

    void EngineDirectionalLight::serialize(BinaryWriter& file) const {
        writeString(file, "dirlight");
        writeString(file, getName());

        writeValue(file, light.getColor());
        writeValue(file, light.getIntensity());
        writeValue(file, light.getDir());
    }

    EngineDirectionalLight::EngineDirectionalLight(BinaryReader& file)
        : EngineEntity(readString(file))
        , light({}, {}, { 0, 0, 1 })
    {
        auto color = readValue<Vec3f>(file);
        auto intensity = readValue<float>(file);
        auto dir = readValue<Vec3f>(file);
        light = DirectionalLight(color, intensity, dir);
    }


    PointLight& EnginePointLight::get() {
        return light;
    }

    std::string EnginePointLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "pos") return toString(light.getPos());
        if (property == "falloff") return toString(light.getFalloff());

        return "";
    }

    void EnginePointLight::setProperty(std::string property, std::string value) {
        setPointLightProp(property, value, light);
        renderNeeded();
    }

    void EnginePointLight::enable() {
        addLight(&light);
        renderNeeded();
    }

    void EnginePointLight::disable() {
        removeLight(&light);
        renderNeeded();
    }

    EngineEntity::H EnginePointLight::copy() const {
        return EngineEntity::make<EnginePointLight>(getName(), light);
    }

    void EnginePointLight::serialize(BinaryWriter& file) const {
        writeString(file, "pointlight");
        writeString(file, getName());

        writeValue(file, light.getColor());
        writeValue(file, light.getIntensity());
        writeValue(file, light.getPos());
        writeValue(file, light.getFalloff());
    }

    EnginePointLight::EnginePointLight(BinaryReader& file)
        : EngineEntity(readString(file))
        , light({}, {}, {}, {})
    {
        auto color = readValue<Vec3f>(file);
        auto intensity = readValue<float>(file);
        auto pos = readValue<Vec3f>(file);
        auto falloff = readValue<float>(file);
        light = PointLight(color, intensity, pos, falloff);
    }


    SpotLight& EngineSpotLight::get() {
        return light;
    }

    std::string EngineSpotLight::getProperty(std::string property) const {
        if (property == "color") return toString(light.getColor());
        if (property == "intensity") return toString(light.getIntensity());

        if (property == "pos") return toString(light.getPos());
        if (property == "falloff") return toString(light.getFalloff());

        if (property == "dir") return toString(light.getDir());
        if (property == "width") return toString(light.getWidth());
        if (property == "falloffexp") return toString(light.getFalloffExp());

        return "";
    }

    void EngineSpotLight::setProperty(std::string property, std::string value) {
        setSpotLightProp(property, value, light);
        renderNeeded();
    }

    void EngineSpotLight::enable() {
        addLight(&light);
        renderNeeded();
    }

    void EngineSpotLight::disable() {
        removeLight(&light);
        renderNeeded();
    }

    EngineEntity::H EngineSpotLight::copy() const {
        return EngineEntity::make<EngineSpotLight>(getName(), light);
    }

    void EngineSpotLight::serialize(BinaryWriter& file) const {
        writeString(file, "spotlight");
        writeString(file, getName());

        writeValue(file, light.getColor());
        writeValue(file, light.getIntensity());
        writeValue(file, light.getPos());
        writeValue(file, light.getDir());
        writeValue(file, light.getFalloff());
        writeValue(file, light.getWidth());
        writeValue(file, light.getFalloffExp());
    }

    EngineSpotLight::EngineSpotLight(BinaryReader& file)
        : EngineEntity(readString(file))
        , light({}, {}, {}, {}, {}, {}, {})
    {
        auto color = readValue<Vec3f>(file);
        auto intensity = readValue<float>(file);
        auto pos = readValue<Vec3f>(file);
        auto dir = readValue<Vec3f>(file);
        auto falloff = readValue<float>(file);
        auto width = readValue<float>(file);
        auto falloffexp = readValue<float>(file);
        light = SpotLight(
            color, intensity, pos, dir, falloff, width, falloffexp
        );
    }


    std::string EngineStaticPhysics::getProperty(std::string property) const {
        if (property == "pos") return toString(pos);
        if (property == "rot") return toString(rot);
        return "";
    }

    void EngineStaticPhysics::setProperty(std::string property, std::string value) {
        if (property == "pos") fromString(value, pos);
        else if (property == "rot") fromString(value, rot);
    }


    EngineEntity::H EngineStaticPhysics::copy() const {
        return EngineEntity::make<EngineStaticPhysics>(getName(), pos, rot);
    }

    void EngineStaticPhysics::serialize(BinaryWriter& file) const {
        writeString(file, "staticphys");

        writeValue(file, pos);
        writeValue(file, rot);
    }

    EngineStaticPhysics::EngineStaticPhysics(BinaryReader& file)
        : EnginePhysicsEntity("x")
    {
        pos = readValue<Vec3f>(file);
        rot = readValue<Vec3f>(file);
    }

    Mat4x4f EngineStaticPhysics::getTransform() const {
        return matmat(rotate(rot), translate(pos));
    }


    Particle& EngineParticle::get() {
        return e;
    }

    std::string EngineParticle::getProperty(std::string property) const {
        return getParticleProp(property, e);
    }

    void EngineParticle::setProperty(std::string property, std::string value) {
        setParticleProp(property, value, e);
    }

    void EngineParticle::enable() {
        addEntity(&e);
    }

    void EngineParticle::disable() {
        removeEntity(&e);
    }

    EngineEntity::H EngineParticle::copy() const {
        return EngineEntity::make<EngineParticle>(getName(), e);
    }

    void EngineParticle::serialize(BinaryWriter& file) const {
        writeString(file, "particle");
        writeString(file, getName());

        writeValue(file, e.pos);
        writeValue(file, e.vel);
        writeValue(file, e.wass);
    }

    EngineParticle::EngineParticle(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
    {
        e.pos = readValue<Vec3f>(file);
        e.vel = readValue<Vec3f>(file);
        e.wass = readValue<float>(file);
    }

    Mat4x4f EngineParticle::getTransform() const {
        return translate(e.pos);
    }

    Particle* EngineParticle::getParticle() {
        return &e;
    }


    RigidBody& EngineRigidBody::get() {
        return e;
    }

    std::string EngineRigidBody::getProperty(std::string property) const {
        return getRigidBodyProp(property, e);
    }

    void EngineRigidBody::setProperty(std::string property, std::string value) {
        setRigidBodyProp(property, value, e);
    }

    void EngineRigidBody::enable() {
        addEntity(&e);
    }

    void EngineRigidBody::disable() {
        removeEntity(&e);
    }

    EngineEntity::H EngineRigidBody::copy() const {
        return EngineEntity::make<EngineRigidBody>(getName(), e);
    }

    void EngineRigidBody::serialize(BinaryWriter& file) const {
        writeString(file, "rigidbody");
        writeString(file, getName());

        writeValue(file, e.pos);
        writeValue(file, e.vel);
        writeValue(file, e.rot);
        writeValue(file, e.rotAxis);
        writeValue(file, e.wass);
        writeValue(file, e.woi);
    }

    EngineRigidBody::EngineRigidBody(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
    {
        e.pos = readValue<Vec3f>(file);
        e.vel = readValue<Vec3f>(file);
        e.rot = readValue<Vec4f>(file);
        e.rotAxis = readValue<Vec3f>(file);
        e.wass = readValue<float>(file);
        e.woi = readValue<Vec3f>(file);
    }

    Mat4x4f EngineRigidBody::getTransform() const {
        return matmat(qtnmat(e.rot), translate(e.pos));
    }

    Particle* EngineRigidBody::getParticle() {
        return &e;
    }

    RigidBody* EngineRigidBody::getRigidBody() {
        return &e;
    }


    ParticleCollider& EngineParticleCollider::get() {
        return e;
    }

    std::string EngineParticleCollider::getProperty(std::string property) const {
        return getParticleProp(property, e.e);
    }

    void EngineParticleCollider::setProperty(std::string property, std::string value) {
        setParticleProp(property, value, e.e);
    }

    void EngineParticleCollider::enable() {
        addEntity(&e.e);
        addCollider(&e);
    }

    void EngineParticleCollider::disable() {
        removeEntity(&e.e);
        removeCollider(&e);
    }

    EngineEntity::H EngineParticleCollider::copy() const {
        return EngineEntity::make<EngineParticleCollider>(getName(), e);
    }

    void EngineParticleCollider::serialize(BinaryWriter& file) const {
        writeString(file, "particlecollider");
        writeString(file, getName());

        writeValue(file, e.e.pos);
        writeValue(file, e.e.vel);
        writeValue(file, e.e.wass);
    }

    EngineParticleCollider::EngineParticleCollider(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
        , e({})
    {
        e.e.pos = readValue<Vec3f>(file);
        e.e.vel = readValue<Vec3f>(file);
        e.e.wass = readValue<float>(file);
    }

    Mat4x4f EngineParticleCollider::getTransform() const {
        return translate(e.e.pos);
    }

    Particle* EngineParticleCollider::getParticle() {
        return &e.e;
    }


    PlaneCollider& EnginePlaneCollider::get() {
        return e;
    }

    std::string EnginePlaneCollider::getProperty(std::string property) const {
        if (property == "normal") return toString(e.n);
        if (property == "dist") return toString(e.d);
        return "";
    }

    void EnginePlaneCollider::setProperty(std::string property, std::string value) {
        if (property == "normal") {
            Vec3f n;
            if (!fromString(value, n)) return;
            if (!n) return;
            e.n = n;
        }
        else if (property == "dist") fromString(value, e.d);
    }

    void EnginePlaneCollider::enable() {
        addCollider(&e);
    }

    void EnginePlaneCollider::disable() {
        removeCollider(&e);
    }

    EngineEntity::H EnginePlaneCollider::copy() const {
        return EngineEntity::make<EnginePlaneCollider>(getName(), e);
    }

    void EnginePlaneCollider::serialize(BinaryWriter& file) const {
        writeString(file, "planecollider");
        writeString(file, getName());

        writeValue(file, e.n);
        writeValue(file, e.d);
    }

    EnginePlaneCollider::EnginePlaneCollider(BinaryReader& file)
        : EngineEntity(readString(file))
        , e({}, {})
    {
        e.n = readValue<Vec3f>(file);
        e.d = readValue<float>(file);
    }


    SphereCollider& EngineSphereCollider::get() {
        return e;
    }

    std::string EngineSphereCollider::getProperty(std::string property) const {
        std::string pres = getRigidBodyProp(property, e.e);
        if (!pres.empty()) return pres;
        if (property == "radius") return toString(e.r);
        return "";
    }

    void EngineSphereCollider::setProperty(std::string property, std::string value) {
        if (setRigidBodyProp(property, value, e.e)) return;
        if (property == "radius") {
            float r;
            if (!fromString(value, r)) return;
            if (r <= 0.0f) return;
            e.r = r;
        }
    }

    void EngineSphereCollider::enable() {
        addEntity(&e.e);
        addCollider(&e);
    }

    void EngineSphereCollider::disable() {
        removeEntity(&e.e);
        removeCollider(&e);
    }

    EngineEntity::H EngineSphereCollider::copy() const {
        return EngineEntity::make<EngineSphereCollider>(getName(), e);
    }

    void EngineSphereCollider::serialize(BinaryWriter& file) const {
        writeString(file, "spherecollider");
        writeString(file, getName());

        writeValue(file, e.e.pos);
        writeValue(file, e.e.vel);
        writeValue(file, e.e.rot);
        writeValue(file, e.e.rotAxis);
        writeValue(file, e.e.wass);
        writeValue(file, e.e.woi);

        writeValue(file, e.r);
    }

    EngineSphereCollider::EngineSphereCollider(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
        , e({}, {})
    {
        e.e.pos = readValue<Vec3f>(file);
        e.e.vel = readValue<Vec3f>(file);
        e.e.rot = readValue<Vec4f>(file);
        e.e.rotAxis = readValue<Vec3f>(file);
        e.e.wass = readValue<float>(file);
        e.e.woi = readValue<Vec3f>(file);

        e.r = readValue<float>(file);
    }

    Mat4x4f EngineSphereCollider::getTransform() const {
        return matmat(qtnmat(e.e.rot), translate(e.e.pos));
    }

    Particle* EngineSphereCollider::getParticle() {
        return &e.e;
    }

    RigidBody* EngineSphereCollider::getRigidBody() {
        return &e.e;
    }


    BoxCollider& EngineBoxCollider::get() {
        return e;
    }

    std::string EngineBoxCollider::getProperty(std::string property) const {
        std::string pres = getRigidBodyProp(property, e.e);
        if (!pres.empty()) return pres;
        if (property == "size") return toString(e.size);
        return "";
    }

    void EngineBoxCollider::setProperty(std::string property, std::string value) {
        if (setRigidBodyProp(property, value, e.e)) return;
        if (property == "size") {
            Vec3f size;
            if (!fromString(value, size)) return;
            if (size <= 0.0f) return;
            e.size = size;
        }
    }

    void EngineBoxCollider::enable() {
        addEntity(&e.e);
        addCollider(&e);
    }

    void EngineBoxCollider::disable() {
        removeEntity(&e.e);
        removeCollider(&e);
    }

    EngineEntity::H EngineBoxCollider::copy() const {
        return EngineEntity::make<EngineBoxCollider>(getName(), e);
    }

    void EngineBoxCollider::serialize(BinaryWriter& file) const {
        writeString(file, "boxcollider");
        writeString(file, getName());

        writeValue(file, e.e.pos);
        writeValue(file, e.e.vel);
        writeValue(file, e.e.rot);
        writeValue(file, e.e.rotAxis);
        writeValue(file, e.e.wass);
        writeValue(file, e.e.woi);

        writeValue(file, e.size);
    }

    EngineBoxCollider::EngineBoxCollider(BinaryReader& file)
        : EnginePhysicsEntity(readString(file))
        , e({}, {})
    {
        e.e.pos = readValue<Vec3f>(file);
        e.e.vel = readValue<Vec3f>(file);
        e.e.rot = readValue<Vec4f>(file);
        e.e.rotAxis = readValue<Vec3f>(file);
        e.e.wass = readValue<float>(file);
        e.e.woi = readValue<Vec3f>(file);

        e.size = readValue<Vec3f>(file);
    }

    Mat4x4f EngineBoxCollider::getTransform() const {
        return matmat(qtnmat(e.e.rot), translate(e.e.pos));
    }

    Particle* EngineBoxCollider::getParticle() {
        return &e.e;
    }

    RigidBody* EngineBoxCollider::getRigidBody() {
        return &e.e;
    }


    DistanceConstraint& EngineDistanceConstraint::get() {
        return c;
    }

    std::string EngineDistanceConstraint::getProperty(std::string property) const {
        if (property == "distance") return toString(c.l0);
        if (property == "compliance") return toString(c.a);
        if (property.size() < 4) return "";
        if (property.substr(0, 3) == "e1.")
            return getParticleProp(property.substr(3), *c.e1);
        if (property.substr(0, 3) == "e2.")
            return getParticleProp(property.substr(3), *c.e2);
        return "";
    }

    void EngineDistanceConstraint::setProperty(std::string property, std::string value) {
        if (property == "distance") {
            float l0;
            if (!fromString(value, l0)) return;
            if (l0 < 0.0f) return;
            c.l0 = l0;
        }
        else if (property == "compliance") {
            float a;
            if (!fromString(value, a)) return;
            if (a < 0.0f) return;
            c.a = a;
        }
        else if (property.size() < 4) return;
        else if (property.substr(0, 3) == "e1.")
            setParticleProp(property.substr(3), value, *c.e1);
        else if (property.substr(0, 3) == "e2.")
            setParticleProp(property.substr(3), value, *c.e2);
    }

    void EngineDistanceConstraint::enable() {
        addConstraint(&c);
    }

    void EngineDistanceConstraint::disable() {
        removeConstraint(&c);
    }

    EngineEntity::H EngineDistanceConstraint::copy() const {
        auto e = EngineEntity::make<EngineDistanceConstraint>(
            getName(), visible, c.l0, c.a, e1, e2
        );
        refs.emplace_back(e1->getPath(), [e](Tree::H e1) {
            e->e1 = EngineEntity::cast<EnginePhysicsEntity>(e1);
            e->c.e1 = e->e1->getParticle();
            assert(e->e1 && e->c.e1);
        });
        refs.emplace_back(e2->getPath(), [e](Tree::H e2) {
            e->e2 = EngineEntity::cast<EnginePhysicsEntity>(e2);
            e->c.e2 = e->e2->getParticle();
            assert(e->e2 && e->c.e2);
        });
        return e;
    }

    void EngineDistanceConstraint::serialize(BinaryWriter& file) const {
        writeString(file, "distanceconstraint");
        writeString(file, getName());

        writeValue(file, visible);
        writeValue(file, c.l0);
        writeValue(file, c.a);
        writeString(file, e1->getPath());
        writeString(file, e2->getPath());
    }

    EngineDistanceConstraint::EngineDistanceConstraint(BinaryReader& file)
        : EngineEntity(readString(file))
    {
        visible = readValue<bool>(file);

        c.l0 = readValue<float>(file);
        c.a = readValue<float>(file);

        std::string e1path = readString(file);
        std::string e2path = readString(file);

        refs.emplace_back(e1path, [this](Tree::H e) {
            e1 = EngineEntity::cast<EnginePhysicsEntity>(e);
            c.e1 = e1->getParticle();
            assert(e1 && c.e1);
        });

        refs.emplace_back(e2path, [this](Tree::H e) {
            e2 = EngineEntity::cast<EnginePhysicsEntity>(e);
            c.e2 = e2->getParticle();
            assert(e2 && c.e2);
        });
    }


    RopeConstraint& EngineRopeConstraint::get() {
        return c;
    }

    std::string EngineRopeConstraint::getProperty(std::string property) const {
        if (property == "distance") return toString(c.l0);
        if (property == "compliance") return toString(c.a);
        if (property == "r1") return toString(c.r1);
        if (property == "r2") return toString(c.r2);
        if (property.size() < 4) return "";
        if (property.substr(0, 3) == "e1.")
            return getRigidBodyProp(property.substr(3), *c.e1);
        if (property.substr(0, 3) == "e2.")
            return getRigidBodyProp(property.substr(3), *c.e2);
        return "";
    }

    void EngineRopeConstraint::setProperty(std::string property, std::string value) {
        if (property == "distance") {
            float l0;
            if (!fromString(value, l0)) return;
            if (l0 < 0.0f) return;
            c.l0 = l0;
        }
        else if (property == "compliance") {
            float a;
            if (!fromString(value, a)) return;
            if (a < 0.0f) return;
            c.a = a;
        }
        else if (property == "r1") fromString(value, c.r1);
        else if (property == "r2") fromString(value, c.r2);
        else if (property.size() < 4) return;
        else if (property.substr(0, 3) == "e1.")
            setRigidBodyProp(property.substr(3), value, *c.e1);
        else if (property.substr(0, 3) == "e2.")
            setRigidBodyProp(property.substr(3), value, *c.e2);
    }

    void EngineRopeConstraint::enable() {
        addConstraint(&c);
    }

    void EngineRopeConstraint::disable() {
        removeConstraint(&c);
    }

    EngineEntity::H EngineRopeConstraint::copy() const {
        auto e = EngineEntity::make<EngineRopeConstraint>(
            getName(), visible, c.l0, c.a, e1, e2, c.r1, c.r2
        );
        refs.emplace_back(e1->getPath(), [e](Tree::H e1) {
            e->e1 = EngineEntity::cast<EnginePhysicsEntity>(e1);
            e->c.e1 = e->e1->getRigidBody();
            assert(e->e1 && e->c.e1);
        });
        refs.emplace_back(e2->getPath(), [e](Tree::H e2) {
            e->e2 = EngineEntity::cast<EnginePhysicsEntity>(e2);
            e->c.e2 = e->e2->getRigidBody();
            assert(e->e2 && e->c.e2);
        });
        return e;
    }

    void EngineRopeConstraint::serialize(BinaryWriter& file) const {
        writeString(file, "ropeconstraint");
        writeString(file, getName());

        writeValue(file, visible);
        writeValue(file, c.l0);
        writeValue(file, c.a);
        writeString(file, e1->getPath());
        writeString(file, e2->getPath());
        writeValue(file, c.r1);
        writeValue(file, c.r2);
    }

    EngineRopeConstraint::EngineRopeConstraint(BinaryReader& file)
        : EngineEntity(readString(file))
    {
        visible = readValue<bool>(file);
        float l0 = readValue<float>(file);
        float a = readValue<float>(file);
        std::string e1path = readString(file);
        std::string e2path = readString(file);
        Vec3f r1 = readValue<Vec3f>(file);
        Vec3f r2 = readValue<Vec3f>(file);

        c = RopeConstraint(l0, a, nullptr, r1, nullptr, r2);

        refs.emplace_back(e1path, [this](Tree::H e) {
            e1 = EngineEntity::cast<EnginePhysicsEntity>(e);
            c.e1 = e1->getRigidBody();
            assert(e1 && c.e1);
        });

        refs.emplace_back(e2path, [this](Tree::H e) {
            e2 = EngineEntity::cast<EnginePhysicsEntity>(e);
            c.e2 = e2->getRigidBody();
            assert(e2 && c.e2);
        });
    }

}
