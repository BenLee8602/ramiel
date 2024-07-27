#pragma once

#include <ramiel/data.h>
#include "triangle.h"

namespace ramiel {

    class EntityBase {
    public:
        virtual void run() const = 0;
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


        virtual void run() const override {
            // run vertex shader
            const std::vector<Vertex>& v_in = mesh.getVertices();
            const size_t numVertex = v_in.size();

            std::vector<typename VertexShader::Vertex_Out> v_out(numVertex);
            for (size_t i = 0; i < numVertex; ++i) {
                v_out[i] = vertexShader(v_in[i]);
                v_out[i].pos = getCameraCoord(v_out[i].pos);
            }

            const std::vector<Vec3u>& triangles = mesh.getTriangles();
            Triangle<typename VertexShader::Vertex_Out, PixelShader> tri(pixelShader);
            for (auto& t : triangles) {
                // backface culling
                if (dot(
                    cross(
                        v_out[t[1]].pos - v_out[t[0]].pos,
                        v_out[t[2]].pos - v_out[t[0]].pos
                    ),  v_out[t[0]].pos
                ) >= 0.0f) continue;
                
                // assemble and draw triangle
                tri.draw(v_out[t[0]], v_out[t[1]], v_out[t[2]]);
            }
        }

    };

}
