#pragma once

#include <ramiel/data.h>
#include "triangle.h"

namespace ramiel {

    class EntityBase {
    public:
        virtual void run() = 0;
    };


    template<class Vertex, class VertexShader, class PixelShader>
    class Entity : public EntityBase {
        const Mesh<Vertex>& mesh;
        VertexShader vertexShader;
        PixelShader pixelShader;
    public:
        Entity(
            const Mesh<Vertex>& mesh,
            const VertexShader& vertexShader,
            const PixelShader& pixelShader
        ) :
            mesh(mesh),
            vertexShader(vertexShader),
            pixelShader(pixelShader)
        {}


        virtual void run() override {
            // run vertex shader
            const std::vector<Vertex>& v_in = mesh.getVertices();
            const size_t numVertex = v_in.size();

            std::vector<typename VertexShader::Vertex_Out> v_out(numVertex);
            for (size_t i = 0; i < numVertex; ++i) {
                v_out[i] = vertexShader(v_in[i]);
                v_out[i].pos = getCameraCoord(v_out[i].pos);
            }

            const std::vector<Vec3u>& triangles = mesh.getTriangles();
            for (auto& t : triangles) {
                draw(pixelShader, v_out[t[0]], v_out[t[1]], v_out[t[2]]);
            }
        }

    };

}
