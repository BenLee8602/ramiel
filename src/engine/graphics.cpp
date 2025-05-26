#include <cassert>
#include <unordered_set>

#include "graphics.h"
#include "entity.h"
using namespace ramiel;

namespace {

    Vec3f backgroundColor = {};
    std::unordered_set<EngineGraphicsEntity*> entities;

}

namespace ramiel {

    Vec3f getBackgroundColor() {
        return backgroundColor;
    }

    void setBackgroundColor(Vec3f color) {
        if (color < 0.0f) return;
        backgroundColor = color;
    }


    void addGraphicsEntity(EngineGraphicsEntity* e) {
        assert(e);
        entities.insert(e);
    }

    void removeGraphicsEntity(EngineGraphicsEntity* e) {
        assert(e);
        entities.erase(e);
    }


    void renderFrame() {
        if (!getRes()[X] || !getRes()[Y]) return;

        std::fill(getColorBuffer(), getColorBuffer() + getBufferSize(), backgroundColor);
        std::fill(getDepthBuffer(), getDepthBuffer() + getBufferSize(), getZ1());

        for (auto& e : entities) {
            e->updatePhys();
            e->get().draw();
        }
    }

}
