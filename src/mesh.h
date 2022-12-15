#pragma once

#include <string>
#include "meshbase.h"
#include "objloader.h"

namespace ramiel {

    template<class Vertex>
    class Mesh : public MeshBase {
        static_assert(
            std::is_base_of_v<Vertex_Mesh, Vertex>,
            "Mesh: invalid vertex type"
        );
        
        std::vector<Vertex> vertices;

    public:
        Mesh(const char* filename, bool loadvt = false, bool loadvn = false) {
            ObjLoader(filename, triangles, vertices, loadvt, loadvn);
        }
        Mesh(std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
            this->triangles = std::move(triangles);
            this->vertices  = std::move(vertices);
        }
        const std::vector<Vertex>& getVertices() const { return vertices; }
    };

}
