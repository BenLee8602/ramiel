#pragma once

#include <string>
#include <functional>
#include <ramiel/file.h>
#include "voidbuffer.h"

namespace ramiel {

    class Mesh {
        VoidBuffer triangles;
        VoidBuffer vertices;

    public:
        Mesh(const Mesh&) = default;
        Mesh(Mesh&&) = default;
        Mesh& operator=(const Mesh&) = default;
        Mesh& operator=(Mesh&&) = default;

        Mesh(const VoidBuffer& triangles, const VoidBuffer& vertices);
        Mesh(VoidBuffer&& triangles, VoidBuffer&& vertices);

        const VoidBuffer& getTriangles() const;
        const VoidBuffer& getVertices() const;
    };


    template<class T>
    Mesh meshFromFile(
        const std::string& filename,
        std::function<T(const Vec3f&, const Vec2f&, const Vec3f&)> makeVertex
    ) {
        std::vector<Vec3u> f;
        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;
        std::unordered_map<Vec3u, size_t, Hash_Vec3u> vertexIndices;

        if (!loadObj(filename, f, v, vt, vn, vertexIndices))
            return Mesh(makeVoidBuffer<char>(), makeVoidBuffer<char>());

        VoidBuffer triangles = makeVoidBuffer<Vec3u>(f.size());
        for (size_t i = 0; i < triangles.getLength(); ++i) {
            Vec3u* tri = static_cast<Vec3u*>(triangles[i]);
            *tri = f[i];
        }

        VoidBuffer vertices = makeVoidBuffer<T>(vertexIndices.size());
        for (auto& [key, value] : vertexIndices) {
            T* vtx = static_cast<T*>(vertices[value]);
            *vtx = makeVertex(v[key[0]], vt[key[1]], vn[key[2]]);
        }
        
        return Mesh(std::move(triangles), std::move(vertices));
    }


    struct MeshVertex {
        Vec3f pos;
        MeshVertex(Vec3f pos = Vec3f()) : pos(pos) {}
        static MeshVertex make(
            const Vec3f& v,
            const Vec2f& vt,
            const Vec3f& vn
        ) { return MeshVertex(v); }
    };

    struct MeshVertexT {
        Vec3f pos;
        Vec2f txt;
        MeshVertexT(Vec3f pos = Vec3f(), Vec2f txt = Vec2f()) : pos(pos), txt(txt) {}
        static MeshVertexT make(
            const Vec3f& v,
            const Vec2f& vt,
            const Vec3f& vn
        ) { return MeshVertexT(v, vt); }
    };

    struct MeshVertexN {
        Vec3f pos;
        Vec3f nml;
        MeshVertexN(Vec3f pos = Vec3f(), Vec3f nml = Vec3f()) : pos(pos), nml(nml) {}
        static MeshVertexN make(
            const Vec3f& v,
            const Vec2f& vt,
            const Vec3f& vn
        ) { return MeshVertexN(v, vn); }
    };

    struct MeshVertexTN {
        Vec3f pos;
        Vec2f txt;
        Vec3f nml;
        MeshVertexTN(
            Vec3f pos = Vec3f(),
            Vec2f txt = Vec2f(),
            Vec3f nml = Vec3f()
        ) : pos(pos), txt(txt), nml(nml) {}
        static MeshVertexTN make(
            const Vec3f& v,
            const Vec2f& vt,
            const Vec3f& vn
        ) { return MeshVertexTN(v, vt, vn); }
    };

}
