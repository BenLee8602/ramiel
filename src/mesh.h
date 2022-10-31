#pragma once

#include <string>
#include <vector>
#include "vec.h"
#include "rotation.h"
#include "vertex.h"

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
        Mesh();
        const std::vector<Vertex>& getVertices() const {
            return vertices;
        }
        virtual void draw() const override {
            
        }
    };

}
