#include "scene.h"

namespace ramiel {

    bool Scene::loadTexture(const char* filename) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[filename]) return false;
        textures[filename] = new TextureRGB(filename);
        return true;
    }

    bool Scene::loadNormalMap(const char* filename) {
        if (!std::ifstream(filename).good()) return false;
        if (textures[filename]) return false;
        textures[filename] = new TextureNML(filename);
        return true;
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

}
