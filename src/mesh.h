#pragma once

#include <string>
#include <vector>
#include "objloader.h"
#include "rotation.h"

namespace ramiel {

    class MeshBase {
		std::vector<Vec3u> tri;
    public:
        MeshBase(
            std::string filename,
            float scale = 1.0f,
            Vec3f pos = vec3f_0,
            Rotation rot = Rotation()
        );
        const std::vector<Vec3u>& getTri() const;
        virtual void pipeline() const = 0;
    };

    template<typename Vertex>
    class Mesh : public MeshBase {
        std::vector<Vertex> vertices;
    public:
        Mesh(
            std::string filename, Vertex(conv*)(const objloader::Vertex),
            float scale = 1.0f, Vec3f pos = vec3f_0, Rotation rot = Rotation()
        ) {
            objloader::load(filename, vertices, )
        }
        const std::vector<Vertex>& getVertices() const {
            return vertices;
        }
        virtual void draw() const override {
            
        }
    };

}
