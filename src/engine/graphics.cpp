#include <cassert>
#include <unordered_set>

#include "graphics.h"
#include "entity.h"
#include "window.h"
using namespace ramiel;

namespace {

    // todo: should probably change color buffer back to
    // float once we do hdr stuff
    std::vector<Vec3ui8> colorBuffer;
    std::vector<float> depthBuffer;

    Vec3ui8 backgroundColor = {};

    std::unordered_set<EngineGraphicsEntity*> entities;

    bool renderIsNeeded = true;

}

namespace ramiel {

    void resizeFrame(Vec2u size) {
        setRes(size);
        size_t bufferSize = getBufferSize();

        colorBuffer = std::vector<Vec3ui8>(bufferSize);
        depthBuffer = std::vector<float>(bufferSize);

        setColorBuffer(colorBuffer.data());
        setDepthBuffer(depthBuffer.data());

        setColorFormat(cfmtRGB888, sizeof(Vec3ui8));

        renderNeeded();
    }


    Vec3ui8 getBackgroundColor() {
        return backgroundColor;
    }

    void setBackgroundColor(Vec3ui8 color) {
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


    void renderNeeded() {
        renderIsNeeded = true;
    }

    void render() {
        if (!getRes()[X] || !getRes()[Y]) return;

        if (!renderIsNeeded) return;
        renderIsNeeded = false;

        std::fill(colorBuffer.begin(), colorBuffer.end(), backgroundColor);
        std::fill(depthBuffer.begin(), depthBuffer.end(), getZ1());

        for (auto& e : entities) {
            e->updatePhys();
            e->get().draw();
        }

        updateFrame();
    }

}
