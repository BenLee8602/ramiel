#pragma once

#include <string>
#include <vector>
#include <array>
#include <functional>

#include <ramiel/math.h>

namespace ramiel {

    struct ObjData {
        using FaceVtx = std::array<uint32_t, 3>;
        using Face = std::array<FaceVtx, 3>;

        std::vector<Face> f;
        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;
    };

    ObjData loadObj(const std::string& filename);

    struct VertexBuffer {
        std::vector<uint32_t> f;
        std::vector<float> v;
    };
    
    VertexBuffer makeVertexBuffer(const ObjData& data);

    using VertexFactory = std::function<void(
        const Vec3f& v,
        const Vec2f& vt,
        const Vec3f& vn,
        float* out
    )>;

    VertexBuffer makeVertexBuffer(
        const ObjData& data,
        size_t vertexSize,
        VertexFactory vertexFactory
    );

    VertexFactory make_v();
    VertexFactory make_vt();
    VertexFactory make_vn();
    VertexFactory make_vtn();
}
