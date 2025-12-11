#pragma once

#include <ramiel/math.h>

namespace ramiel {

    class EngineGraphicsEntity;

    void resizeFrame(Vec2u size);

    Vec3ui8* getColorBuf();
    float* getDepthBuf();
    Vec4ui8* getMenuBuf();

    Vec3ui8 getBackgroundColor();
    void setBackgroundColor(Vec3ui8 color);

    void addGraphicsEntity(EngineGraphicsEntity* e);
    void removeGraphicsEntity(EngineGraphicsEntity* e);

    void renderNeeded();
    void menuRenderNeeded();
    void render();

}
