#pragma once

#include <cstdint>

namespace ramiel {

    class Entity;

    void addGraphicsEntity(Entity* e);
    void removeGraphicsEntity(Entity* e);

    void renderFrame(uint8_t* frame);

}
