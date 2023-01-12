#pragma once

#include "vec.h"

namespace ramiel { 

    struct Vertex_Mesh {
        Vec3f pos;
        Vertex_Mesh() : pos(vec3f_0) {}
        Vertex_Mesh(const Vec3f& pos) : pos(pos) {}
        Vertex_Mesh(
            const Vec3f&,
            const Vec2f&
        ) { throw std::string("shouldnt be called!"); }
        Vertex_Mesh(
            const Vec3f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
        Vertex_Mesh(
            const Vec3f&,
            const Vec2f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
    };

    struct Vertex_Mesh_T : public Vertex_Mesh {
        Vec2f texture;
        Vertex_Mesh_T() : texture(vec2f_0) {}
        Vertex_Mesh_T(
            const Vec3f& pos,
            const Vec2f& texture
        ) :
            Vertex_Mesh(pos),
            texture(texture)
        {}
        Vertex_Mesh_T(const Vec3f&) {
            throw std::string("shouldnt be called!");
        }
        Vertex_Mesh_T(
            const Vec3f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
        Vertex_Mesh_T(
            const Vec3f&,
            const Vec2f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
    };

    struct Vertex_Mesh_N : public Vertex_Mesh {
        Vec3f normal;
        Vertex_Mesh_N() : normal(vec3f_0) {}
        Vertex_Mesh_N(
            const Vec3f& pos,
            const Vec3f& normal
        ) :
            Vertex_Mesh(pos),
            normal(normal)
        {}
        Vertex_Mesh_N(const Vec3f&) {
            throw std::string("shouldnt be called!");
        }
        Vertex_Mesh_N(
            const Vec3f&,
            const Vec2f&
        ) { throw std::string("shouldnt be called!"); }
        Vertex_Mesh_N(
            const Vec3f&,
            const Vec2f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
    };

    struct Vertex_Mesh_TN : public Vertex_Mesh {
        Vec2f texture;
        Vec3f normal;
        Vertex_Mesh_TN() : texture(vec2f_0), normal(vec3f_0) {}
        Vertex_Mesh_TN(
            const Vec3f& pos,
            const Vec2f& texture,
            const Vec3f& normal
        ) :
            Vertex_Mesh(pos),
            texture(texture),
            normal(normal)
        {}
        Vertex_Mesh_TN(const Vec3f& pos) {
            throw std::string("shouldnt be called!");
        }
        Vertex_Mesh_TN(
            const Vec3f&,
            const Vec2f&
        ) { throw std::string("shouldnt be called!"); }
        Vertex_Mesh_TN(
            const Vec3f&,
            const Vec3f&
        ) { throw std::string("shouldnt be called!"); }
    };

}
