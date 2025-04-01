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

    Vec3f getBackgroundColor() {
        std::lock_guard lock(mutex);
        return backgroundColor;
    }

    void setBackgroundColor(Vec3f color) {
        assert(color > 0.0f);
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


    void renderFrame(uint8_t* frame) {
        std::lock_guard lock(mutex);

        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), backgroundColor);
        std::fill(getDepthBuffer(), getDepthBuffer() + getBufferSize(), getZ1());

        for (auto& e : entities) e->draw();

        assert(frame);
        auto color = getColorBuffer();
        for (size_t i = 0; i < getBufferSize(); ++i) {
            *frame++ = std::min(color[i][B], 255.0f);
            *frame++ = std::min(color[i][G], 255.0f);
            *frame++ = std::min(color[i][R], 255.0f);
        }
    }

}
