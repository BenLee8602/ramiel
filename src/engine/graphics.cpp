#include <cassert>
#include <unordered_set>

#include <ramiel/graphics.h>
#include "graphics.h"
using namespace ramiel;

namespace {

    std::unordered_set<Entity*> entities;

}

namespace ramiel {

    void addGraphicsEntity(Entity* e) {
        assert(e);
        entities.insert(e);
    }

    void removeGraphicsEntity(Entity* e) {
        assert(e);
        entities.erase(e);
    }


    void renderFrame(uint8_t* frame) {
        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), Vec3f{});
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
