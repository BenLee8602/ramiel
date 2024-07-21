#include "ramiel.h"
using namespace ramiel;

namespace {

    Vec3f ambientLight = Vec3f();

    std::unordered_map<std::string, MeshBase*> meshes;
    std::unordered_map<std::string, Texture*>  textures;

    std::vector<EntityBase*> entities;
    std::vector<Light*> lights;
    std::vector<Effect*> effects;

    std::vector<Kinematics*> dynamicObjects;
    std::vector<Collider*> colliders;

}

namespace ramiel {

    void loadMesh(const char* meshName, MeshBase* mesh) {
        meshes[meshName] = mesh;
    }

    MeshBase* getMesh(const char* meshName) {
        return meshes[meshName];
    }


    bool loadTexture(const char* filename, const char* textureName, Mat4x4f format) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[textureName]) return false;
        textures[textureName] = new Texture(filename, format);
        return true;
    }

    Texture* getTexture(const char* textureName) {
        return (*textures.find(textureName)).second;
    }


    void addEntity(EntityBase* entity) {
        entities.push_back(entity);
    }


    const Vec3f& getAmbientLight() {
        return ambientLight;
    }

    void setAmbientLight(const Vec3f& color) {
        ambientLight = color;
    }

    void addLight(Light* light) {
        lights.push_back(light);
    }

    LightingList getLightingList() {
        return LightingList(ambientLight, lights);
    }

    LightingListSpecular getLightingList(
        uint16_t specularExponent,
        float specularIntensity
    ) {
        return LightingListSpecular(
            ambientLight,
            lights,
            getPos(),
            specularExponent,
            specularIntensity
        );
    }


    void addEffect(Effect* effect) {
        effects.push_back(effect);
    }


    void renderFrame() {
        resetBuffers();
        for (auto& e : entities) e->run();
        for (auto& e : effects)  e->run();
        clampColorBuffer();
    }


    bool addDynamicObject(Kinematics* dynamicObject) {
        if (!dynamicObject) return false;
        dynamicObjects.emplace_back(dynamicObject);
        return true;
    }


    bool removeDynamicObject(Kinematics* dynamicObject) {
        auto i = std::find(dynamicObjects.begin(), dynamicObjects.end(), dynamicObject);
        if (i == dynamicObjects.end()) return false;
        dynamicObjects.erase(i);
        return true;
    }


    bool addCollider(Collider* collider) {
        if (!collider) return false;
        colliders.emplace_back(collider);
        return true;
    }


    bool removeCollider(Collider* collider) {
        auto i = std::find(colliders.begin(), colliders.end(), collider);
        if (i == colliders.end()) return false;
        colliders.erase(i);
        return true;
    }

    
    void simulatePhysics(float dtime) {
        for (auto& o : dynamicObjects) o->step(dtime);

        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                CollisionHandler_P handler = getCollisionHandler(colliders[i], colliders[j]);
                if (handler && handler->detect()) handler->resolve();
            }
        }
    }


    void destroy() {
        dynamicObjects = std::vector<Kinematics*>();
        colliders = std::vector<Collider*>();

        for (auto& mesh : meshes) delete mesh.second;
        for (auto& texture : textures) delete texture.second;
        
        for (auto& entity : entities) delete entity;
        for (auto& light : lights) delete light;
    }

}
