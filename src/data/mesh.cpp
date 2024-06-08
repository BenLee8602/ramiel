#include <vector>
#include "mesh.h"

namespace ramiel {

    Mesh::Mesh(const VoidBuffer& triangles, const VoidBuffer& vertices)
        : triangles(triangles)
        , vertices(vertices)
    {}

    Mesh::Mesh(VoidBuffer&& triangles, VoidBuffer&& vertices)
        : triangles(triangles)
        , vertices(vertices)
    {}


    const VoidBuffer& Mesh::getTriangles() const {
        return triangles;
    }

    const VoidBuffer& Mesh::getVertices() const {
        return vertices;
    }

}
