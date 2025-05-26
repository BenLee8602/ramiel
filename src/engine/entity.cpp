#include "entity.h"
#include "graphics.h"
#include "serialize.h"
#include "command.h"
using namespace ramiel;

namespace {

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

    bool EngineEntity::enableAll(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        if (ee) ee->enable();
        e->forEachKid(enableAll);
        return true;
    }

    bool EngineEntity::disableAll(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        if (ee) ee->disable();
        e->forEachKid(disableAll);
        return true;
    }


    Tree::H EngineEntity::copyAll(Tree::H e) {
        auto ee = EngineEntity::cast<EngineEntity>(e);
        Tree::H out = ee ? ee->copy() : Tree::make(e->getName());
        e->forEachKid([out](Tree::H e) {
            out->insert(copyAll(e));
            return true;
        });
        return out;
    }


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
    }

    void EngineGraphicsEntity::enable() {
        addGraphicsEntity(this);
        phys->enable();
    }

    void EngineGraphicsEntity::disable() {
        removeGraphicsEntity(this);
        phys->disable();
    }

    EngineEntity::H EngineGraphicsEntity::copy() const {
        return EngineEntity::make<EngineGraphicsEntity>(
            getName(),
            EngineEntity::cast<EngineMesh>(mesh->copy()),
            EngineEntity::cast<EnginePhysicsEntity>(phys->copy()),
            std::unique_ptr<EngineVertexShaderBase>(vs->copy()),
            std::unique_ptr<EnginePixelShaderBase>(ps->copy())
        );
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
    }

    void EngineDirectionalLight::enable() {
        addLight(&light);
    }

    void EngineDirectionalLight::disable() {
        removeLight(&light);
    }

    EngineEntity::H EngineDirectionalLight::copy() const {
        return EngineEntity::make<EngineDirectionalLight>(getName(), light);
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
    }

    void EnginePointLight::enable() {
        addLight(&light);
    }

    void EnginePointLight::disable() {
        removeLight(&light);
    }

    EngineEntity::H EnginePointLight::copy() const {
        return EngineEntity::make<EnginePointLight>(getName(), light);
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
    }

    void EngineSpotLight::enable() {
        addLight(&light);
    }

    void EngineSpotLight::disable() {
        removeLight(&light);
    }

    EngineEntity::H EngineSpotLight::copy() const {
        return EngineEntity::make<EngineSpotLight>(getName(), light);
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

    Mat4x4f EngineParticle::getTransform() const {
        return translate(e.pos);
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

    Mat4x4f EngineRigidBody::getTransform() const {
        return matmat(qtnmat(e.rot), translate(e.pos));
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

    Mat4x4f EngineParticleCollider::getTransform() const {
        return translate(e.e.pos);
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

    Mat4x4f EngineSphereCollider::getTransform() const {
        return matmat(qtnmat(e.e.rot), translate(e.e.pos));
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

    Mat4x4f EngineBoxCollider::getTransform() const {
        return matmat(qtnmat(e.e.rot), translate(e.e.pos));
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
        return EngineEntity::make<EngineDistanceConstraint>(getName(), visible, c);
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
        return EngineEntity::make<EngineRopeConstraint>(getName(), visible, c);
    }

}
