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
            "objloader: invalid vertex type"
        );

        typedef void(ObjLoader::*LoadLine)(std::string&, std::istringstream&);
        typedef void(ObjLoader::*SetVertex)(const Vec3u&);

        std::vector<Vec3u>& f;
        std::vector<Vertex>& v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;

        size_t fCount;
        size_t vCount;
        size_t vtCount;
        size_t vnCount;

        std::vector<uint32_t> polygon;

        LoadLine loadLine;
        ParsePolygonVertex parsePolygonVertex;
        SetVertex setVertex;


        void loadLine_v(std::string& type, std::istringstream& str) {
            if (type == "v") v.emplace_back(Vertex(parseV3(str)));
            else if (type == "f") {
                while (str.rdbuf()->in_avail()) {
                    Vec3u i = parsePolygonVertex(str) - 1;
                    polygon.emplace_back(i[0]);
                    (this->*setVertex)(i);
                }
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    f.emplace_back(Vec3u{ polygon[0], polygon[i], polygon[i + 1] });
                polygon.clear();
            }
        }

        void loadLine_vt(std::string& type, std::istringstream& str) {
            if (type == "vt") vt.emplace_back(parseV2(str));
            else loadLine_v(type, str);
        }

        void loadLine_vn(std::string& type, std::istringstream& str) {
            if (type == "vn") vn.emplace_back(parseV3(str));
            else loadLine_v(type, str);
        }

        void loadLine_vtn(std::string& type, std::istringstream& str) {
            if (type == "vt") vt.emplace_back(parseV2(str));
            else if (type == "vn") vn.emplace_back(parseV3(str));
            else loadLine_v(type, str);
        }


        void setVertex_v(const Vec3u& i) {
            return;
        }

        void setVertex_vt(const Vec3u& i) {
            v[i[0]].setTexture(vt[i[1]]);
        }

        void setVertex_vn(const Vec3u& i) {
            v[i[0]].setNormal(vn[i[2]]);
        }

        void setVertex_vtn(const Vec3u& i) {
            v[i[0]].setTexture(vt[i[1]]);
            v[i[0]].setNormal(vn[i[2]]);
        }


    public:
        ObjLoader(
            const char* filename,
            std::vector<Vec3u>& f,
            std::vector<Vertex>& v,
            bool loadvt = false,
            bool loadvn = false
        ) :
            f(f), v(v),
            fCount(0),
            vCount(0),
            vtCount(0),
            vnCount(0),
            loadLine(nullptr),
            parsePolygonVertex(nullptr),
            setVertex(nullptr)
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

            v.reserve(vCount);
            f.reserve(fCount);

            bool vtGood = (loadvt && vtCount && (
                std::is_base_of_v<Vertex_Mesh_T, Vertex> ||
                std::is_base_of_v<Vertex_Mesh_TN, Vertex>
            ));
            bool vnGood = (loadvn && vtCount && (
                std::is_base_of_v<Vertex_Mesh_N, Vertex> ||
                std::is_base_of_v<Vertex_Mesh_TN, Vertex>
            ));

            if (vtGood) {
                vt.reserve(vtCount);
                if (vnGood) {
                    vn.reserve(vnCount);
                    loadLine  = loadLine_vtn;
                    setVertex = setVertex_vtn;
                } else {
                    loadLine  = loadLine_vt;
                    setVertex = setVertex_vt;
                }
            } else if (vnGood) {
                vn.reserve(vnCount);
                loadLine  = loadLine_vn;
                setVertex = setVertex_vn;
            } else {
                loadLine  = loadLine_v;
                setVertex = setVertex_v;
            }

            file.clear();
            file.seekg(0, std::ios::beg);

            while (std::getline(file, line)) {
                std::istringstream str(std::move(line));
                str >> line;
                (this->*loadLine)(line, str);
            }
        }
    };

}
