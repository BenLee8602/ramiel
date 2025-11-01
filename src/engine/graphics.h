#pragma once

#include <ramiel/graphics.h>

namespace ramiel {

    class EngineGraphicsEntity;

    void resizeFrame(Vec2u size);

    Vec3ui8 getBackgroundColor();
    void setBackgroundColor(Vec3ui8 color);

    void addGraphicsEntity(EngineGraphicsEntity* e);
    void removeGraphicsEntity(EngineGraphicsEntity* e);

    void renderNeeded();
    void render();

}
