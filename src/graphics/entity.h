#pragma once

#include <memory>
#include <ramiel/data.h>
#include "triangle.h"

namespace ramiel {

    class Mesh;
    class VertexShaderBase;
    class PixelShaderBase;

    class Entity {
    public:
        Entity(
            std::shared_ptr<Mesh> mesh,
            std::unique_ptr<VertexShaderBase>&& vertexShader,
            std::unique_ptr<PixelShaderBase>&& pixelShader
        );
        operator bool();
        void draw();

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

    private:
        std::shared_ptr<Mesh> mesh;
        std::unique_ptr<VertexShaderBase> vertexShader;
        std::unique_ptr<PixelShaderBase> pixelShader;
    };

}
