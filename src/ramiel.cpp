#include "ramiel.h"
#include "graphics/vertexshader.h"
#include "graphics/pixelshader.h"
using namespace ramiel;

namespace {

    Vec3f backgroundColor = Vec3f();
    Vec3f ambientLight = Vec3f();

    std::vector<Entity> entities;
    std::vector<Light*> lights;

    std::vector<Kinematics*> dynamicObjects;
    std::vector<Collider*> colliders;

}

namespace ramiel {

    const Vec3f& getBackgroundColor() {
        return backgroundColor;
    }

    void setBackgroundColor(const Vec3f& color) {
        backgroundColor = color;
    }


    void addEntity(Entity&& entity) {
        entities.emplace_back(std::move(entity));
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

    Vec3f getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        unsigned specularExponent,
        float specularIntensity
    ) {
        Vec3f light = ambientLight;
        for (auto l : lights) {
            light += l->getLight(pos, normal, specularExponent, specularIntensity);
        }
        return light;
    }


    void renderFrame() {
        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), backgroundColor);
        std::fill(getDepthBuffer(), getDepthBuffer() + getBufferSize(), getZ1());

        for (auto& e : entities) e.draw();

        auto color = getColorBuffer();
        for (size_t i = 0; i < getBufferSize(); ++i)
            color[i] = min(color[i], 255.0f);
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

        for (auto& light : lights) delete light;
    }

}
