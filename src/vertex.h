#pragma once

#include "vec.h"

namespace ramiel {

    struct Vertex_In_P {
        Vec3f pos;
    };

    struct Vertex_In_PN {
        Vec3f pos;
        Vec3f nml;
    };

    struct Vertex_In_PT {
        Vec3f pos;
        Vec3f txt;
    };

    struct Vertex_In_PNT {
        Vec3f pos;
        Vec3f nml;
        Vec3f txt;
    };


    struct Vertex_Out_PerTri {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vertex_Out_PerTri operator+(const Vertex_Out_PerTri& other) const;
        Vertex_Out_PerTri operator-(const Vertex_Out_PerTri& other) const;
        Vertex_Out_PerTri operator*(float n) const;
        Vertex_Out_PerTri operator/(float n) const;
        Vertex_Out_PerTri& operator+=(const Vertex_Out_PerTri& other);
    };

    struct Vertex_Out_PerTri_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vec2f texturePos;
        Vertex_Out_PerTri_Textured operator+(const Vertex_Out_PerTri_Textured& other) const;
        Vertex_Out_PerTri_Textured operator-(const Vertex_Out_PerTri_Textured& other) const;
        Vertex_Out_PerTri_Textured operator*(float n) const;
        Vertex_Out_PerTri_Textured operator/(float n) const;
        Vertex_Out_PerTri_Textured& operator+=(const Vertex_Out_PerTri_Textured& other);
    };

    struct Vertex_Out_PerVertex {
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f color;
        Vertex_Out_PerVertex operator+(const Vertex_Out_PerVertex& other) const;
        Vertex_Out_PerVertex operator-(const Vertex_Out_PerVertex& other) const;
        Vertex_Out_PerVertex operator*(float n) const;
        Vertex_Out_PerVertex operator/(float n) const;
        Vertex_Out_PerVertex& operator+=(const Vertex_Out_PerVertex& other);
    };

    struct Vertex_Out_PerVertex_Textured {
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f color;
        float zinv;
        Vec2f texturePos;
        Vertex_Out_PerVertex_Textured operator+(const Vertex_Out_PerVertex_Textured& other) const;
        Vertex_Out_PerVertex_Textured operator-(const Vertex_Out_PerVertex_Textured& other) const;
        Vertex_Out_PerVertex_Textured operator*(float n) const;
        Vertex_Out_PerVertex_Textured operator/(float n) const;
        Vertex_Out_PerVertex_Textured& operator+=(const Vertex_Out_PerVertex_Textured& other);
    };

    struct Vertex_Out_PerPixel {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vertex_Out_PerPixel operator+(const Vertex_Out_PerPixel& other) const;
        Vertex_Out_PerPixel operator-(const Vertex_Out_PerPixel& other) const;
        Vertex_Out_PerPixel operator*(float n) const;
        Vertex_Out_PerPixel operator/(float n) const;
        Vertex_Out_PerPixel& operator+=(const Vertex_Out_PerPixel& other);
    };

    struct Vertex_Out_PerPixel_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        float zinv;
        Vec2f texturePos;
        Vertex_Out_PerPixel_Textured operator+(const Vertex_Out_PerPixel_Textured& other) const;
        Vertex_Out_PerPixel_Textured operator-(const Vertex_Out_PerPixel_Textured& other) const;
        Vertex_Out_PerPixel_Textured operator*(float n) const;
        Vertex_Out_PerPixel_Textured operator/(float n) const;
        Vertex_Out_PerPixel_Textured& operator+=(const Vertex_Out_PerPixel_Textured& other);
    };

    struct Vertex_Out_PerPixel_Smooth {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f normal;
        float zinv;
        Vertex_Out_PerPixel_Smooth operator+(const Vertex_Out_PerPixel_Smooth& other) const;
        Vertex_Out_PerPixel_Smooth operator-(const Vertex_Out_PerPixel_Smooth& other) const;
        Vertex_Out_PerPixel_Smooth operator*(float n) const;
        Vertex_Out_PerPixel_Smooth operator/(float n) const;
        Vertex_Out_PerPixel_Smooth& operator+=(const Vertex_Out_PerPixel_Smooth& other);
    };

    struct Vertex_Out_PerPixel_Smooth_Textured {
        Vec3f worldPos;
        Vec3f cameraPos;
        Vec2f screenPos;
        Vec3f normal;
        float zinv;
        Vec2f texturePos;
        Vertex_Out_PerPixel_Smooth_Textured operator+(const Vertex_Out_PerPixel_Smooth_Textured& other) const;
        Vertex_Out_PerPixel_Smooth_Textured operator-(const Vertex_Out_PerPixel_Smooth_Textured& other) const;
        Vertex_Out_PerPixel_Smooth_Textured operator*(float n) const;
        Vertex_Out_PerPixel_Smooth_Textured operator/(float n) const;
        Vertex_Out_PerPixel_Smooth_Textured& operator+=(const Vertex_Out_PerPixel_Smooth_Textured& other);
    };

}
