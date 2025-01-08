#include <cassert>

#include <ramiel/file.h>
#include "mesh.h"

namespace {

    std::vector<uint8_t> calcAttrPos(const std::vector<uint8_t>& attrType) {
        size_t attrSize = attrType.size();
        std::vector<uint8_t> attrOutPos(attrSize);
        uint8_t pos = 0;
        for (size_t i = 0; i < attrSize; i++) {
            assert(attrType[i]);
            attrOutPos[i] = pos;
            pos += attrType[i];
        }
        return attrOutPos;
    }

    size_t calcVertexSize(const std::vector<uint8_t>& attrType) {
        size_t size = 0;
        for (auto& t : attrType) {
            assert(t);
            size += t;
        }
        return size;
    }

}

namespace ramiel {

    Mesh::Mesh(
        const std::vector<float>& vertices,
        const std::vector<uint32_t>& triangles,
        const std::vector<uint8_t>& attrType
    )
        : vertices(vertices)
        , triangles(triangles)
        , attrOutType(attrType)
    {
        initMetadata();
    }


    Mesh::Mesh(
        std::vector<float>&& vertices,
        std::vector<uint32_t>&& triangles,
        const std::vector<uint8_t>& attrType
    )
        : vertices(std::move(vertices))
        , triangles(std::move(triangles))
        , attrOutType(attrType)
    {
        initMetadata();
    }


    Mesh::Mesh(const std::string& filename) {
        ObjData data = loadObj(filename);

        attrOutType.push_back(3);
        if (data.vt.size() > 1) attrOutType.push_back(2);
        if (data.vn.size() > 1) attrOutType.push_back(3);

        VertexBuffer vertexBuffer = makeVertexBuffer(data);
        vertices = std::move(vertexBuffer.v);
        triangles = std::move(vertexBuffer.f);

        initMetadata();
    }


    float* Mesh::vtxBegin() {
        return &*vertices.begin();
    }

    float* Mesh::vtxEnd() {
        return &*vertices.end();
    }
    
    uint32_t* Mesh::triBegin() {
        return &*triangles.begin();
    }
    
    uint32_t* Mesh::triEnd() {
        return &*triangles.end();
    }
    
    
    const float* Mesh::vtxBegin() const {
        return &*vertices.begin();
    }
    
    const float* Mesh::vtxEnd() const {
        return &*vertices.end();
    }
    
    const uint32_t* Mesh::triBegin() const {
        return &*triangles.begin();
    }
    
    const uint32_t* Mesh::triEnd() const {
        return &*triangles.end();
    }


    const std::vector<uint8_t>& Mesh::getAttrOutType() const {
        return attrOutType;
    }

    const std::vector<uint8_t>& Mesh::getAttrOutPos() const {
        return attrOutPos;
    }

    size_t Mesh::getVertexSize() const {
        return vertexSize;
    }
    

    size_t Mesh::getVertexCount() const {
        return vertexCount;
    }

    size_t Mesh::getTriangleCount() const {
        return triangleCount;
    }


    void Mesh::initMetadata() {
        assert(!vertices.empty());
        assert(!triangles.empty());
        assert(!attrOutType.empty());
        
        this->attrOutPos = calcAttrPos(attrOutType);
        this->vertexSize = calcVertexSize(attrOutType);
        this->vertexCount = vertices.size() / vertexSize;
        this->triangleCount = triangles.size() / 3;

        assert(vertices.size() % vertexSize == 0);
        assert(triangles.size() % 3 == 0);
    }

}
