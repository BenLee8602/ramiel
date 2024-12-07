#pragma once

#include <vector>
#include <string>
#include <ramiel/file.h>

namespace ramiel {

    class Mesh {
        friend class Entity;

    public:
        Mesh(
            const std::vector<uint8_t>& attr,
            const std::vector<float>& vertices,
            const std::vector<uint32_t>& triangles
        );
        Mesh(
            const std::vector<uint8_t>& attr,
            std::vector<float>&& vertices,
            std::vector<uint32_t>&& triangles
        );

        const std::vector<float>& getVertices() const;
        const std::vector<uint32_t>& getTriangles() const;

    private:
        std::vector<uint8_t> getAttrOutType() const;
        std::vector<uint8_t> getAttrOutPos() const;
        size_t getAttrOutSize() const;

    private:
        std::vector<uint8_t> attr;
        std::vector<float> vertices;
        std::vector<uint32_t> triangles;
    };

}
