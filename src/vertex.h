#pragma once

#include "vec.h"

namespace ramiel {

    struct Vertex_PerTri {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vertex_PerTri operator+(const Vertex_PerTri& other) const;
        Vertex_PerTri operator-(const Vertex_PerTri& other) const;
        Vertex_PerTri operator*(float n) const;
        Vertex_PerTri operator/(float n) const;
        Vertex_PerTri& operator+=(const Vertex_PerTri& other);
    };

    struct Vertex_PerTri_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vec2f texturePos;
        Vertex_PerTri_Textured operator+(const Vertex_PerTri_Textured& other) const;
        Vertex_PerTri_Textured operator-(const Vertex_PerTri_Textured& other) const;
        Vertex_PerTri_Textured operator*(float n) const;
        Vertex_PerTri_Textured operator/(float n) const;
        Vertex_PerTri_Textured& operator+=(const Vertex_PerTri_Textured& other);
    };

    struct Vertex_PerVertex {
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f color;
        Vertex_PerVertex operator+(const Vertex_PerVertex& other) const;
        Vertex_PerVertex operator-(const Vertex_PerVertex& other) const;
        Vertex_PerVertex operator*(float n) const;
        Vertex_PerVertex operator/(float n) const;
        Vertex_PerVertex& operator+=(const Vertex_PerVertex& other);
    };

    struct Vertex_PerVertex_Textured {
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f color;
        float zinv;
        Vec2f texturePos;
        Vertex_PerVertex_Textured operator+(const Vertex_PerVertex_Textured& other) const;
        Vertex_PerVertex_Textured operator-(const Vertex_PerVertex_Textured& other) const;
        Vertex_PerVertex_Textured operator*(float n) const;
        Vertex_PerVertex_Textured operator/(float n) const;
        Vertex_PerVertex_Textured& operator+=(const Vertex_PerVertex_Textured& other);
    };

    struct Vertex_PerPixel {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vertex_PerPixel operator+(const Vertex_PerPixel& other) const;
        Vertex_PerPixel operator-(const Vertex_PerPixel& other) const;
        Vertex_PerPixel operator*(float n) const;
        Vertex_PerPixel operator/(float n) const;
        Vertex_PerPixel& operator+=(const Vertex_PerPixel& other);
    };

    struct Vertex_PerPixel_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vec2f texturePos;
        Vertex_PerPixel_Textured operator+(const Vertex_PerPixel_Textured& other) const;
        Vertex_PerPixel_Textured operator-(const Vertex_PerPixel_Textured& other) const;
        Vertex_PerPixel_Textured operator*(float n) const;
        Vertex_PerPixel_Textured operator/(float n) const;
        Vertex_PerPixel_Textured& operator+=(const Vertex_PerPixel_Textured& other);
    };

    struct Vertex_PerPixel_Smooth {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f normal;
        float zinv;
        Vertex_PerPixel_Smooth operator+(const Vertex_PerPixel_Smooth& other) const;
        Vertex_PerPixel_Smooth operator-(const Vertex_PerPixel_Smooth& other) const;
        Vertex_PerPixel_Smooth operator*(float n) const;
        Vertex_PerPixel_Smooth operator/(float n) const;
        Vertex_PerPixel_Smooth& operator+=(const Vertex_PerPixel_Smooth& other);
    };

    struct Vertex_PerPixel_Smooth_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f normal;
        float zinv;
        Vec2f texturePos;
        Vertex_PerPixel_Smooth_Textured operator+(const Vertex_PerPixel_Smooth_Textured& other) const;
        Vertex_PerPixel_Smooth_Textured operator-(const Vertex_PerPixel_Smooth_Textured& other) const;
        Vertex_PerPixel_Smooth_Textured operator*(float n) const;
        Vertex_PerPixel_Smooth_Textured operator/(float n) const;
        Vertex_PerPixel_Smooth_Textured& operator+=(const Vertex_PerPixel_Smooth_Textured& other);
    };

}
