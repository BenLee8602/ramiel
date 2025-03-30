#pragma once

#include <ramiel/graphics.h>

namespace ramiel {

    class Entity;

    Vec3f getBackgroundColor();
    void setBackgroundColor(Vec3f color);

    void addGraphicsEntity(Entity* e);
    void removeGraphicsEntity(Entity* e);

    void renderFrame(uint8_t* frame);

}
