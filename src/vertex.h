#pragma once

#include "vec.h"

namespace ramiel {

    struct Vertex_PerTri {
        Vec2f scrPos;
        float z;
        Vertex_PerTri operator+(const Vertex_PerTri& other) const;
        Vertex_PerTri operator-(const Vertex_PerTri& other) const;
        Vertex_PerTri operator*(float n) const;
        Vertex_PerTri operator/(float n) const;
        Vertex_PerTri& operator+=(const Vertex_PerTri& other);
    };

    struct Vertex_PerTri_Textured {
        Vec2f scrPos;
        float z;
        float zinv;
        float uv;
        Vertex_PerTri_Textured operator+(const Vertex_PerTri_Textured& other) const;
        Vertex_PerTri_Textured operator-(const Vertex_PerTri_Textured& other) const;
        Vertex_PerTri_Textured operator*(float n) const;
        Vertex_PerTri_Textured operator/(float n) const;
        Vertex_PerTri_Textured& operator+=(const Vertex_PerTri_Textured& other);
    };

    struct Vertex_PerVertex {
        Vec2f scrPos;
        float z;
        Vec3f color;
        Vertex_PerVertex operator+(const Vertex_PerVertex& other) const;
        Vertex_PerVertex operator-(const Vertex_PerVertex& other) const;
        Vertex_PerVertex operator*(float n) const;
        Vertex_PerVertex operator/(float n) const;
        Vertex_PerVertex& operator+=(const Vertex_PerVertex& other);
    };

    struct Vertex_PerVertex_Textured {
        Vec2f scrPos;
        float z;
        Vec3f color;
        float zinv;
        float uv;
        Vertex_PerVertex_Textured operator+(const Vertex_PerVertex_Textured& other) const;
        Vertex_PerVertex_Textured operator-(const Vertex_PerVertex_Textured& other) const;
        Vertex_PerVertex_Textured operator*(float n) const;
        Vertex_PerVertex_Textured operator/(float n) const;
        Vertex_PerVertex_Textured& operator+=(const Vertex_PerVertex_Textured& other);
    };

    struct Vertex_PerPixel {
        Vec2f scrPos;
        float z;
        Vec3f wrlPos;
        float zinv;
        Vertex_PerPixel operator+(const Vertex_PerPixel& other) const;
        Vertex_PerPixel operator-(const Vertex_PerPixel& other) const;
        Vertex_PerPixel operator*(float n) const;
        Vertex_PerPixel operator/(float n) const;
        Vertex_PerPixel& operator+=(const Vertex_PerPixel& other);
    };

    struct Vertex_PerPixel_Textured {
        Vec2f scrPos;
        float z;
        Vec3f wrlPos;
        float zinv;
        float uv;
        Vertex_PerPixel_Textured operator+(const Vertex_PerPixel_Textured& other) const;
        Vertex_PerPixel_Textured operator-(const Vertex_PerPixel_Textured& other) const;
        Vertex_PerPixel_Textured operator*(float n) const;
        Vertex_PerPixel_Textured operator/(float n) const;
        Vertex_PerPixel_Textured& operator+=(const Vertex_PerPixel_Textured& other);
    };

    struct Vertex_PerPixel_S {
        Vec2f scrPos;
        float z;
        Vec3f wrlPos;
        Vec3f normal;
        float zinv;
        Vertex_PerPixel_S operator+(const Vertex_PerPixel_S& other) const;
        Vertex_PerPixel_S operator-(const Vertex_PerPixel_S& other) const;
        Vertex_PerPixel_S operator*(float n) const;
        Vertex_PerPixel_S operator/(float n) const;
        Vertex_PerPixel_S& operator+=(const Vertex_PerPixel_S& other);
    };

    struct Vertex_PerPixel_S_Textured {
        Vec2f scrPos;
        float z;
        Vec3f wrlPos;
        Vec3f normal;
        float zinv;
        float uv;
        Vertex_PerPixel_S_Textured operator+(const Vertex_PerPixel_S_Textured& other) const;
        Vertex_PerPixel_S_Textured operator-(const Vertex_PerPixel_S_Textured& other) const;
        Vertex_PerPixel_S_Textured operator*(float n) const;
        Vertex_PerPixel_S_Textured operator/(float n) const;
        Vertex_PerPixel_S_Textured& operator+=(const Vertex_PerPixel_S_Textured& other);
    };

}
