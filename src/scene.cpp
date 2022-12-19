#include "scene.h"

namespace ramiel {

    bool Scene::loadTexture(const char* filename, const char* textureName) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[textureName]) return false;
        textures[textureName] = new TextureRGB(filename);
        return true;
    }

    bool Scene::loadNormalMap(const char* filename, const char* normalMapName) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[normalMapName]) return false;
        textures[normalMapName] = new TextureNML(filename);
        return true;
    }

    const Texture* Scene::getTexture(const char* textureName) const {
        return (*textures.find(textureName)).second;
    }


    void Scene::addLight(Light* light) {
        lights.push_back(light);
    }

    void Scene::addEffect(Effect effect) {
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
            camera,
            specularExponent,
            specularIntensity
        );
    }


    void Scene::renderFrame() {
        camera.resetBuffers();
        for (auto& e : entities) e->draw(camera);
        for (auto& e : effects) e(camera);
        camera.clampColorBuffer();
    }


    Scene::~Scene() {
        for (auto& mesh : meshes) delete mesh.second;
        for (auto& texture : textures) delete texture.second;
        
        for (auto& entity : entities) delete entity;
        for (auto& light : lights) delete light;
    }

}
