#include <ramiel/file.h>
#include "mesh.h"

namespace ramiel {

    Mesh::Mesh(
        const std::vector<uint8_t>& attr,
        const std::vector<float>& vertices,
        const std::vector<uint32_t>& triangles
    ) {
        this->attr = attr;
        this->vertices = vertices;
        this->triangles = triangles;
    }


    Mesh::Mesh(
        const std::vector<uint8_t>& attr,
        std::vector<float>&& vertices,
        std::vector<uint32_t>&& triangles
    ) {
        this->attr = attr;
        this->vertices = std::move(vertices);
        this->triangles = std::move(triangles);
    }


    Mesh::Mesh(const std::string& filename) {
        ObjData data = loadObj(filename);

        attr.push_back(3);
        if (data.vt.size() > 1) attr.push_back(2);
        if (data.vn.size() > 1) attr.push_back(3);

        VertexBuffer vertexBuffer = makeVertexBuffer(data);
        vertices = std::move(vertexBuffer.v);
        triangles = std::move(vertexBuffer.f);
    }


    const std::vector<float>& Mesh::getVertices() const {
        return vertices;
    }

    const std::vector<uint32_t>& Mesh::getTriangles() const {
        return triangles;
    }


    std::vector<uint8_t> Mesh::getAttrOutType() const {
        return attr;
    }

    std::vector<uint8_t> Mesh::getAttrOutPos() const {
        size_t attrSize = attr.size();
        std::vector<uint8_t> attrOutPos(attrSize);
        uint8_t pos = 0;
        for (size_t i = 0; i < attrSize; i++) {
            attrOutPos[i] = pos;
            pos += attr[i];
        }
        return attrOutPos;
    }

    size_t Mesh::getAttrOutSize() const {
        size_t size = 0;
        for (auto& t : attr) {
            size += t;
        }
        return size;
    }

}
