#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "vertex.h"
#include "rotation.h"


namespace ramiel {

    Vec2f parseV2(std::istream& str);
    Vec3f parseV3(std::istream& str);

    typedef Vec3u(*ParsePolygonVertex)(std::istream&);
    Vec3u parsePolygonVertex_v(std::istream& str);
    Vec3u parsePolygonVertex_vt(std::istream& str);
    Vec3u parsePolygonVertex_vn(std::istream& str);
    Vec3u parsePolygonVertex_vtn(std::istream& str);


    template<class Vertex>
    class ObjLoader {
        static_assert(
            std::is_base_of_v<Vertex_Mesh, Vertex>,
            "invalid vertex type given to obj loader"
        );

    protected:
        std::string filename;
        std::vector<Vec3u>& triangles;
        std::vector<Vertex>& vertices;
        size_t fCount;
        size_t vCount;
        size_t vtCount;
        size_t vnCount;
        std::vector<uint32_t> polygon;
        ParsePolygonVertex parsePolygonVertex;

        virtual void allocate() {
            triangles.reserve(fCount);
            vertices.reserve(vCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const {
            return { parseV3(str) };
        }

        virtual void setVertexAttributes(const Vec3u& v) {
            return;
        }

        virtual void loadLine(std::string& type, std::istringstream& str) {
            if (type == "v") vertices.emplace_back(constructVertex(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail()) {
                    Vec3u v = parsePolygonVertex(str) - 1;
                    polygon.emplace_back(v[0]);
                    setVertexAttributes(v);
                }
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back(Vec3u{ polygon[0], polygon[i], polygon[i + 1] });
                polygon.clear();
            }
        }

    public:
        ObjLoader(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) : 
            filename(filename),
            triangles(triangles),
            vertices(vertices),
            fCount(0),
            vCount(0),
            vtCount(0),
            vnCount(0),
            parsePolygonVertex(nullptr)
        {
            std::ifstream file(filename);
            if (!file.is_open()) return;

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream str(std::move(line));
                str >> line;
                if (line == "v") ++vCount;
                else if (line == "f") {
                    size_t nVertices = 0;
                    while (str >> line) ++nVertices;
                    fCount += nVertices - 2;
                }
                else if (line == "vt") ++vtCount;
                else if (line == "vn") ++vnCount;
            }

            if (vtCount) {
                if (vnCount) parsePolygonVertex = parsePolygonVertex_vtn;
                else parsePolygonVertex = parsePolygonVertex_vt;
            }
            else if (vnCount) parsePolygonVertex = parsePolygonVertex_vn;
            else parsePolygonVertex = parsePolygonVertex_v;
        }


        void load() {
            std::ifstream file(filename);
            if (!file.is_open()) return;
            allocate();

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream str(std::move(line));
                str >> line;
                loadLine(line, str);
            }
        }
    };


    template<class Vertex>
    class ObjLoader_T : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_T, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;
        using ObjLoader<Vertex>::vtCount;

    protected:
        std::vector<Vec2f> textures;

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            textures.reserve(vtCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec2f_0 };
        }

        virtual void setVertexAttributes(const Vec3u& v) override {
            if (textures.capacity() && textures[v[1]]) vertices[v[0]].texture = textures[v[1]];
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vt") textures.emplace_back(parseV2(str));
        }
    
    public:
        ObjLoader_T(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) : ObjLoader<Vertex>(filename, triangles, vertices) {}
    };


    template<class Vertex>
    class ObjLoader_N : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_N, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;
        using ObjLoader<Vertex>::vnCount;

    protected:
        std::vector<Vec3f> normals;

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            normals.reserve(vnCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec3f_0 };
        }

        virtual void setVertexAttributes(const Vec3u& v) override {
            if (normals.capacity() && normals[v[2]]) vertices[v[0]].normal = normals[v[2]];
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vn") normals.emplace_back(parseV3(str));
        }
    
    public:
        ObjLoader_N(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) : ObjLoader<Vertex>(filename, triangles, vertices) {}
    };


    template<class Vertex>
    class ObjLoader_TN : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_TN, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;
        using ObjLoader<Vertex>::vtCount;
        using ObjLoader<Vertex>::vnCount;

    protected:
        std::vector<Vec2f> textures;
        std::vector<Vec3f> normals;

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            textures.reserve(vtCount);
            normals.reserve(vnCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec2f_0, vec3f_0 };
        }

        virtual void setVertexAttributes(const Vec3u& v) override {
            if (textures.capacity() && textures[v[1]]) vertices[v[0]].texture = textures[v[1]];
            if (normals.capacity()  && normals[v[2]])  vertices[v[0]].normal  = normals[v[2]];
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vt") textures.emplace_back(parseV2(str));
            else if (type == "vn") normals.emplace_back(parseV3(str));
        }
    
    public:
        ObjLoader_TN(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) : ObjLoader<Vertex>(filename, triangles, vertices) {}
    };

}
