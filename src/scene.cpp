#include "scene.h"

namespace ramiel {

    static bool loadTexture(
        std::unordered_map<std::string, Texture*>& textures,
        const char* filename,
        const char* textureName,
        bool isNormalMap
    ) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[textureName]) return false;
        textures[textureName] = new Texture(filename, false);
        return true;
    }

    bool Scene::loadTexture(const char* filename, const char* textureName) {
        return ramiel::loadTexture(textures, filename, textureName, false);
    }

    bool Scene::loadNormalMap(const char* filename, const char* normalMapName) {
        return ramiel::loadTexture(textures, filename, normalMapName, true);
    }

    const Texture* Scene::getTexture(const char* textureName) const {
        return (*textures.find(textureName)).second;
    }


    void Scene::addLight(Light* light) {
        lights.push_back(light);
    }

    void Scene::addEffect(Effect* effect) {
        effects.push_back(effect);
    }


    LightingList Scene::getLightingList() const {
        return LightingList(ambientLight, lights);
    }

    LightingListSpecular Scene::getLightingList(
        uint16_t specularExponent,
        float specularIntensity
    ) const {
        return LightingListSpecular(
            ambientLight,
            lights,
            camera.pos,
            specularExponent,
            specularIntensity
        );
    }


    void Scene::renderFrame() {
        camera.resetBuffers();
        for (auto& e : entities) e->run(camera);
        for (auto& e : effects)  e->run(camera);
        camera.clampColorBuffer();
    }


    bool Scene::addDynamicObject(Dynamics* dynamicObject) {
        if (!dynamicObject) return false;
        dynamicObjects.emplace_back(dynamicObject);
        return true;
    }


    bool Scene::removeDynamicObject(Dynamics* dynamicObject) {
        auto i = std::find(dynamicObjects.begin(), dynamicObjects.end(), dynamicObject);
        if (i == dynamicObjects.end()) return false;
        dynamicObjects.erase(i);
        return true;
    }


    bool Scene::addCollider(Collider* collider) {
        if (!collider) return false;
        colliders.emplace_back(collider);
        return true;
    }


    bool Scene::removeCollider(Collider* collider) {
        auto i = std::find(colliders.begin(), colliders.end(), collider);
        if (i == colliders.end()) return false;
        colliders.erase(i);
        return true;
    }

    
    void Scene::simulatePhysics(float dtime) {
        for (auto& o : dynamicObjects) o->step(dtime);

        for (size_t i = 0; i < colliders.size(); ++i) {
            for (size_t j = i + 1; j < colliders.size(); ++j) {
                colliders[i]->collideWith(colliders[j]);
            }
        }
    }


    Scene::~Scene() {
        dynamicObjects = std::vector<Dynamics*>();
        colliders = std::vector<Collider*>();

        for (auto& mesh : meshes) delete mesh.second;
        for (auto& texture : textures) delete texture.second;
        
        for (auto& entity : entities) delete entity;
        for (auto& light : lights) delete light;
    }

}
