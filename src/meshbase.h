#pragma once

#include <vector>
#include "vec.h"

namespace ramiel {

    class MeshBase {
    protected:
        std::vector<Vec3u> triangles;
    public:
        const std::vector<Vec3u>& getTriangles() const { return triangles; }
        virtual ~MeshBase() {}
    };

}
