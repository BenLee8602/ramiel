#pragma once

#include <ramiel/data.h>

namespace ramiel {

    class Mesh;
    class VertexShaderBase;
    class PixelShaderBase;

    class Entity {
    public:
        Entity();
        Entity(
            Mesh* mesh,
            VertexShaderBase* vertexShader,
            PixelShaderBase* pixelShader
        );
        operator bool();
        void draw();

        Entity(const Entity&) = delete;
        Entity& operator=(const Entity&) = delete;

        Entity(Entity&&) = default;
        Entity& operator=(Entity&&) = default;

        Mesh* getMesh() const;
        VertexShaderBase* getVertexShader() const;
        PixelShaderBase* getPixelShader() const;

    private:
        Mesh* mesh;
        VertexShaderBase* vertexShader;
        PixelShaderBase* pixelShader;
    };

}
