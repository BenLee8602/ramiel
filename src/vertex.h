#pragma once

#include "vec.h"

namespace ramiel {

    struct Vertex_Mesh {
        Vec3f pos;
        Vertex_Mesh(const Vec3f& pos) : pos(pos) {}
        void setTexture(const Vec2f& texture) {
            throw std::string("Vertex_Mesh: cant set texture property");
        }
        void setNormal(const Vec3f& normal) {
            throw std::string("Vertex_Mesh: cant set normal property");
        }
    };

    struct Vertex_Mesh_T : public Vertex_Mesh {
        Vec2f texture;
        Vertex_Mesh_T(
            const Vec3f& pos,
            const Vec2f& texture = vec2f_0
        ) :
            Vertex_Mesh(pos),
            texture(texture)
        {}
        void setTexture(const Vec2f& texture) {
            this->texture = texture;
        }
        void setNormal(const Vec3f& normal) {
            throw std::string("Vertex_Mesh_T: cant set normal property");
        }
    };

    struct Vertex_Mesh_N : public Vertex_Mesh {
        Vec3f normal;
        Vertex_Mesh_N(
            const Vec3f& pos,
            const Vec3f& normal = vec3f_0
        ) :
            Vertex_Mesh(pos),
            normal(normal)
        {}
        void setTexture(const Vec2f& texture) {
            throw std::string("Vertex_Mesh_N: cant set texture property");
        }
        void setNormal(const Vec3f& normal) {
            this->normal = normal;
        }
    };

    struct Vertex_Mesh_TN : public Vertex_Mesh {
        Vec2f texture;
        Vec3f normal;
        Vertex_Mesh_TN(
            const Vec3f& pos,
            const Vec2f& texture = vec2f_0,
            const Vec3f& normal = vec3f_0
        ) :
            Vertex_Mesh(pos),
            texture(texture),
            normal(normal)
        {}
        void setTexture(const Vec2f& texture) {
            this->texture = texture;
        }
        void setNormal(const Vec3f& normal) {
            this->normal = normal;
        }
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
