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

}
