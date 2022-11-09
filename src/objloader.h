#pragma once

#include <string>
#include <vector>
#include <sstream>
#include "vertex.h"
#include "rotation.h"


namespace ramiel {

    inline Vec3f parseV3(std::istringstream& str) {
        Vec3f v;
        str >> v[0] >> v[1] >> v[2];
        return v;
    }

    inline Vec2f parseV2(std::istringstream& str) {
        Vec2f v;
        str >> v[0] >> v[1];
        return v;
    }


    template<class Vertex>
    class RealObjLoader {

    };


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
        std::vector<Vec3u> polygon;

        virtual void countLine(std::string& type, std::istringstream& str) {
            std::string v;
            if (type == "v") ++vCount;
            else if (type == "f") {
                size_t nVertices = 0;
                while (str >> v) ++nVertices;
                fCount += nVertices - 2;
            }
        }

        virtual void allocate() {
            triangles.reserve(fCount);
            vertices.reserve(vCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const {
            return { parseV3(str) };
        }

        virtual Vec3u parsePolygonVertex(std::istringstream& str) const {
            Vec3u v = vec3u_0;
            str >> v[0];
            return v;
        }

        virtual void setVertexAttributes() {
            return;
        }

        virtual void loadLine(std::string& type, std::istringstream& str) {
            if (type == "v") vertices.emplace_back(constructVertex(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail())
                    polygon.emplace_back(parsePolygonVertex(str) - 1);
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back(Vec3u{ polygon[0][0], polygon[i][0], polygon[i + 1][0] });
                setVertexAttributes();
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
            vCount(0)
        {
            std::ifstream file(filename);
            if (!file.is_open()) return;

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream str(std::move(line));
                str >> line;
                countLine(line, str);
            }
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
    class ObjLoader_Texture : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_T, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;

    protected:
        std::vector<Vec2f> textures;
        size_t vtCount;
    
        virtual void countLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::countLine(type, str);
            if (type == "vt") ++vtCount;
        }

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            textures.reserve(vtCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec2f_0 };
        }

        virtual Vec3u parsePolygonVertex(std::istringstream& str) const override {
            char junk;
            Vec3u v = vec3u_0;
            str >> v[0] >> junk >> v[1];
            return v;
        }

        virtual void setVertexAttributes() override {
            for (size_t i = 0; i < polygon.size(); ++i)
                if (!vertices[polygon[i][0]].texture)
                    vertices[polygon[i][0]].texture = textures[polygon[i][1]];
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vt") textures.emplace_back(parseV2(str));
        }
    
    public:
        ObjLoader_Texture(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) :
            ObjLoader<Vertex>(filename, triangles, vertices),
            vtCount(0)
        {}
    };


    template<class Vertex>
    class ObjLoader_Normal : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_N, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;

    protected:
        std::vector<Vec3f> normals;
        size_t vnCount;

        virtual void countLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::countLine(type, str);
            if (type == "vn") ++vnCount;
        }

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            normals.reserve(vnCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec3f_0 };
        }

        virtual Vec3u parsePolygonVertex(std::istringstream& str) const override {
            char junk;
            Vec3u v = vec3u_0;
            str >> v[0] >> junk >> junk >> v[1];
            return v;
        }

        virtual void setVertexAttributes() override {
            for (size_t i = 0; i < polygon.size(); ++i)
                if (!vertices[polygon[i][0]].normal)
                    vertices[polygon[i][0]].normal = normals[polygon[i][1]];
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vn") normals.emplace_back(parseV3(str));
        }
    
    public:
        ObjLoader_Normal(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) :
            ObjLoader<Vertex>(filename, triangles, vertices),
            vnCount(0)
        {}
    };


    template<class Vertex>
    class ObjLoader_TextureNormal : public ObjLoader<Vertex> {
        static_assert(
            std::is_base_of_v<Vertex_Mesh_TN, Vertex>,
            "invalid vertex type given to obj loader"
        );
        using ObjLoader<Vertex>::vertices;
        using ObjLoader<Vertex>::polygon;

    protected:
        std::vector<Vec2f> textures;
        std::vector<Vec3f> normals;
        size_t vtCount;
        size_t vnCount;

        virtual void countLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::countLine(type, str);
            if (type == "vt") ++vtCount;
            else if (type == "vn") ++vnCount;
        }

        virtual void allocate() override {
            ObjLoader<Vertex>::allocate();
            textures.reserve(vtCount);
            normals.reserve(vnCount);
        }

        virtual Vertex constructVertex(std::istringstream& str) const override {
            return { parseV3(str), vec2f_0, vec3f_0 };
        }

        virtual Vec3u parsePolygonVertex(std::istringstream& str) const override {
            char junk;
            Vec3u v = vec3u_0;
            str >> v[0] >> junk >> v[1] >> junk >> v[2];
            return v;
        }

        virtual void setVertexAttributes() override {
            for (size_t i = 0; i < polygon.size(); ++i) {
                if (!vertices[polygon[i][0]].texture)
                    vertices[polygon[i][0]].texture = textures[polygon[i][1]];
                if (!vertices[polygon[i][0]].normal)
                    vertices[polygon[i][0]].normal = normals[polygon[i][1]];
            }
        }

        virtual void loadLine(std::string& type, std::istringstream& str) override {
            ObjLoader<Vertex>::loadLine(type, str);
            if (type == "vt") textures.emplace_back(parseV2(str));
            else if (type == "vn") normals.emplace_back(parseV3(str));
        }
    
    public:
        ObjLoader_TextureNormal(
            std::string filename,
            std::vector<Vec3u>& triangles,
            std::vector<Vertex>& vertices
        ) :
            ObjLoader<Vertex>(filename, triangles, vertices),
            vtCount(0),
            vnCount(0)
        {}
    };

    /*
    template<class Vertex>
    bool loadV(std::string filename, std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
        static_assert(std::is_base_of_v<Vertex, Vertex_Mesh>, "invalid vertex type given to obj loader"); // diff: put in class decl

        std::ifstream file(filename);
        if (!file.is_open()) return false;

        // diff: move to constructor
        size_t fCount = 0;
        size_t vCount = 0;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            // diff: func
            if (type == "v") ++vCount;
            else if (type == "f") while (str >> type) ++fCount;
        }

        // diff: func
        triangles.reserve(fCount);
        vertices.reserve(vCount);

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<Vec3u> polygon; // diff: make all vec3f
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") vertices.emplace_back({ parseV3(str) }); // diff: func
            else if (type == "f") {
                while (str.rdbuf()->in_avail())
                    polygon.emplace_back(parsePolygonVertex(str));
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back({ polygon[0][0], polygon[i][0], polygon[i + 1][0] });
                // diff: func
                polygon.clear();
            }
        }
    }


    template<class Vertex>
    bool loadVT(std::string filename, std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
        static_assert(std::is_base_of_v<Vertex_Mesh_T>, "invalid vertex type given to obj loader");

        std::ifstream file(filename);
        if (!file.is_open()) return false;

        size_t fCount = 0;
        size_t vCount = 0;
        size_t tCount = 0;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") ++vCount;
            else if (type == "vt") ++tCount;
            else if (type == "f") while (str >> type) ++fCount;
        }

        triangles.reserve(fCount);
        vertices.reserve(vCount);
        std::vector<Vec2f> textures;
        textures.reserve(tCount);

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<Vec3u> polygon;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") vertices.emplace_back({ parseV3(str), vec3f_0 });
            if (type == "vt") textures.emplace_back(parseV2(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail())
                    polygon.emplace_back(parse_v(str) - 1);
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back({ polygon[0][0], polygon[i][0], polygon[i + 1][0] });
                for (size_t i = 0; i < polygon.size(); ++i)
                    if (!vertices[polygon[i][0]].texture)
                        vertices[polygon[i][0]].texture = textures[polygon[i][1]];
                polygon.clear();
            }
        }
    }


    template<class Vertex>
    bool loadVN(std::string filename, std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
        static_assert(std::is_base_of_v<Vertex_Mesh_N>, "invalid vertex type given to obj loader");

        std::ifstream file(filename);
        if (!file.is_open()) return false;

        size_t fCount = 0;
        size_t vCount = 0;
        size_t nCount = 0;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") ++vCount;
            else if (type == "vn") ++nCount;
            else if (type == "f") while (str >> type) ++fCount;
        }

        triangles.reserve(fCount);
        vertices.reserve(vCount);
        std::vector<Vec3f> normals;
        normals.reserve(nCount);

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<Vec3u> polygon;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") vertices.emplace_back({ parseV3(str), vec2f_0 });
            if (type == "vn") normals.emplace_back(parseV3(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail())
                    polygon.emplace_back(parse_v(str) - 1);
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back({ polygon[0][0], polygon[i][0], polygon[i + 1][0] });
                for (size_t i = 0; i < polygon.size(); ++i)
                    if (!vertices[polygon[i][0]].normal)
                        vertices[polygon[i][0]].normal = normals[polygon[i][1]];
                polygon.clear();
            }
        }
    }


    template<class Vertex>
    bool loadVTN(std::string filename, std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices) {
        static_assert(std::is_base_of_v<Vertex_Mesh_TN>, "invalid vertex type given to obj loader");

        std::ifstream file(filename);
        if (!file.is_open()) return false;

        size_t fCount = 0;
        size_t vCount = 0;
        size_t tCount = 0;
        size_t nCount = 0;

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") ++vCount;
            else if (type == "vt") ++tCount;
            else if (type == "vn") ++nCount;
            else if (type == "f") while (str >> type) ++fCount;
        }

        triangles.reserve(fCount);
        vertices.reserve(vCount);
        std::vector<Vec3f> normals;
        std::vector<Vec2f> textures;
        normals.reserve(nCount);
        textures.reserve(tCount);

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<Vec3u> polygon;
        while (std::getline(file, line)) {
            std::istringstream str(std::move(line));

            std::string type;
            str >> type;

            if (type == "v") vertices.emplace_back({ parseV3(str), vec3f_0, vec2f_0 });
            if (type == "vt") textures.emplace_back(parseV2(str));
            if (type == "vn") normals.emplace_back(parseV3(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail())
                    polygon.emplace_back(parse_v(str) - 1);
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    triangles.emplace_back({ polygon[0][0], polygon[i][0], polygon[i + 1][0] });
                for (size_t i = 0; i < polygon.size(); ++i) {
                    if (!vertices[polygon[i][0]].texture)
                        vertices[polygon[i][0]].texture = textures[polygon[i][1]];
                    if (!vertices[polygon[i][0]].normal)
                        vertices[polygon[i][0]].normal = normals[polygon[i][2]];
                }
                polygon.clear();
            }
        }
    }*/

}
