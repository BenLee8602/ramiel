#pragma once

#include <vector>
#include <string>

namespace ramiel {

    class Mesh {
    public:
        Mesh(
            const std::vector<float>& vertices,
            const std::vector<uint32_t>& triangles,
            const std::vector<uint8_t>& attrType
        );
        Mesh(
            std::vector<float>&& vertices,
            std::vector<uint32_t>&& triangles,
            const std::vector<uint8_t>& attrType
        );
        Mesh(const std::string& filename);

        Mesh(const Mesh&) = default;
        Mesh(Mesh&&) = default;

        Mesh& operator=(const Mesh&) = default;
        Mesh& operator=(Mesh&&) = default;

        float* vtxBegin();
        float* vtxEnd();
        uint32_t* triBegin();
        uint32_t* triEnd();
        
        const float* vtxBegin() const;
        const float* vtxEnd() const;
        const uint32_t* triBegin() const;
        const uint32_t* triEnd() const;

        const std::vector<uint8_t>& getAttrOutType() const;
        const std::vector<uint8_t>& getAttrOutPos() const;
        size_t getVertexSize() const;

        size_t getVertexCount() const;
        size_t getTriangleCount() const;

    private:
        std::vector<float> vertices;
        std::vector<uint32_t> triangles;

        std::vector<uint8_t> attrOutType;
        std::vector<uint8_t> attrOutPos;
        size_t vertexSize;

        size_t vertexCount;
        size_t triangleCount;

        void initMetadata();
    };

}
