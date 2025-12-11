#include <cassert>
#include <unordered_set>

#include <ramiel/graphics.h>
#include "graphics.h"
#include "entity.h"
#include "window.h"
#include "menu.h"
using namespace ramiel;

namespace {

    // todo: should probably change color buffer back to
    // float once we do hdr stuff
    std::vector<Vec3ui8> colorBuffer;
    std::vector<float> depthBuffer;

    std::vector<Vec4ui8> menuBuffer;

    Vec3ui8 backgroundColor = {};

    std::unordered_set<EngineGraphicsEntity*> entities;

    bool renderIsNeeded = true;
    bool menuRenderIsNeeded = true;


    void renderScene() {
        if (!renderIsNeeded) return;
        renderIsNeeded = false;

        setColorBuffer(colorBuffer.data());
        setDepthBuffer(depthBuffer.data());
        setColorFormat(cfmtRGB888, sizeof(Vec3ui8));

        std::fill(colorBuffer.begin(), colorBuffer.end(), backgroundColor);
        std::fill(depthBuffer.begin(), depthBuffer.end(), getZ1());

        for (auto& e : entities) {
            e->updatePhys();
            e->get().draw();
        }

        setColorBuffer(nullptr);
        setDepthBuffer(nullptr);
        setColorFormat(nullptr, 0);
    }


    void renderMenu() {
        if (!menuRenderIsNeeded) return;
        menuRenderIsNeeded = false;
        
        setColorBuffer(menuBuffer.data());
        setDepthBuffer(nullptr);
        setColorFormat(cfmtRGBA8888, sizeof(Vec4ui8));

        Menu::currentRender();

        setColorBuffer(nullptr);
        setColorFormat(nullptr, 0);
    }

}

namespace ramiel {

    void resizeFrame(Vec2u size) {
        setRes(size);
        size_t bufferSize = getBufferSize();

        colorBuffer = std::vector<Vec3ui8>(bufferSize);
        depthBuffer = std::vector<float>(bufferSize);
        menuBuffer = std::vector<Vec4ui8>(bufferSize);

        renderNeeded();
        menuRenderNeeded();
    }


    Vec3ui8* getColorBuf() {
        return colorBuffer.data();
    }

    float* getDepthBuf() {
        return depthBuffer.data();
    }

    Vec4ui8* getMenuBuf() {
        return menuBuffer.data();
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

    void menuRenderNeeded() {
        menuRenderIsNeeded = true;
    }

    void render() {
        if (!getRes()[X] || !getRes()[Y]) return;
        if (!renderIsNeeded && !menuRenderIsNeeded) return;

        renderScene();
        renderMenu();

        updateFrame();
    }

}
