#pragma once

#include <ramiel/graphics.h>

namespace ramiel {

    class EngineGraphicsEntity;

    Vec3f getBackgroundColor();
    void setBackgroundColor(Vec3f color);

    void addGraphicsEntity(EngineGraphicsEntity* e);
    void removeGraphicsEntity(EngineGraphicsEntity* e);

    void renderNeeded();
    void render();

}
