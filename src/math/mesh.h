#pragma once

#include <string>

#include "vec.h"
#include "meshvertex.h"

namespace ramiel {

    class MeshBase {
    protected:
        std::vector<Vec3u> triangles;
    public:
        const std::vector<Vec3u>& getTriangles() const { return triangles; }
        virtual ~MeshBase() {}
    };


    template<class Vertex>
    class Mesh : public MeshBase {
        static_assert(
            std::is_base_of_v<Vertex_Mesh, Vertex>,
            "Mesh: invalid vertex type"
        );
        
        std::vector<Vertex> vertices;

    public:
        Mesh(std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
            this->triangles = triangles;
            this->vertices  = vertices;
        }
        Mesh(std::vector<Vec3u>&& triangles, std::vector<Vertex>&& vertices) {
            this->triangles = triangles;
            this->vertices  = vertices;
        }
        const std::vector<Vertex>& getVertices() const { return vertices; }
    };

}
