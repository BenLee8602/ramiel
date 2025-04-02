#include <cassert>
#include <unordered_set>
#include <mutex>

#include "graphics.h"
using namespace ramiel;

namespace {

    Vec3f backgroundColor = {};
    std::unordered_set<Entity*> entities;

    std::mutex mutex;

}

namespace ramiel {

    Vec2u getCameraRes() {
        std::lock_guard lock(mutex);
        return getRes();
    }

    void setCameraRes(Vec2u res) {
        if (res[X] == 0 || res[Y] == 0) return;
        std::lock_guard lock(mutex);
        setRes(res);
    }


    float getCameraAspectRatio() {
        std::lock_guard lock(mutex);
        return getAspectRatio();
    }


    Vec3f getCameraPos() {
        std::lock_guard lock(mutex);
        return getPos();
    }

    void setCameraPos(Vec3f pos) {
        std::lock_guard lock(mutex);
        setPos(pos);
    }


    Vec3f getCameraRot() {
        std::lock_guard lock(mutex);
        return getRot();
    }

    void setCameraRot(Vec3f rot) {
        std::lock_guard lock(mutex);
        setRot(rot);
    }


    float getCameraFov() {
        std::lock_guard lock(mutex);
        return getFov();
    }

    void setCameraFov(float fov) {
        if (fov < 1e-6f || fov > 180.0f) return;
        std::lock_guard lock(mutex);
        setFov(fov);
    }


    float getCameraFocalLength() {
        std::lock_guard lock(mutex);
        return getFocalLen();
    }

    void setCameraFocalLength(float focalLength) {
        if (focalLength < 1e-6f) return;
        std::lock_guard lock(mutex);
        setFocalLen(focalLength);
    }


    Vec3f getAmbientLightColor() {
        std::lock_guard lock(mutex);
        return getAmbientLight();
    }

    void setAmbientLightColor(Vec3f color) {
        if (color < 0.0f) return;
        std::lock_guard lock(mutex);
        setAmbientLight(color);
    }


    Vec3f getBackgroundColor() {
        std::lock_guard lock(mutex);
        return backgroundColor;
    }

    void setBackgroundColor(Vec3f color) {
        if (color < 0.0f) return;
        std::lock_guard lock(mutex);
        backgroundColor = color;
    }


    void addGraphicsEntity(Entity* e) {
        assert(e);
        std::lock_guard lock(mutex);
        entities.insert(e);
    }

    void removeGraphicsEntity(Entity* e) {
        assert(e);
        std::lock_guard lock(mutex);
        entities.erase(e);
    }


    void addGraphicsLight(Light* l) {
        assert(l);
        std::lock_guard lock(mutex);
        addLight(l);
    }

    void removeGraphicsLight(Light* l) {
        assert(l);
        std::lock_guard lock(mutex);
        removeLight(l);
    }


    void renderFrame() {
        std::lock_guard lock(mutex);

        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), backgroundColor);
        std::fill(getDepthBuffer(), getDepthBuffer() + getBufferSize(), getZ1());

        for (auto& e : entities) e->draw();
    }

}
