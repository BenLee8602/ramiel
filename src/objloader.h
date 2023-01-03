#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include "meshvertex.h"
#include "rotation.h"


namespace ramiel {

    struct Hash_Vec3u {
        size_t operator()(const Vec3u& key) const {
            return key[0] << 16 + key[1] << 8 + key[2];
        }
    };


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
        typedef Vertex(ObjLoader::*MakeVertex)(const Vec3u&) const;

        std::vector<Vertex>& v_out;
        std::vector<Vec3u>& f_out;

        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;

        size_t fCount;
        size_t vCount;
        size_t vtCount;
        size_t vnCount;

        std::vector<uint32_t> polygon;
        std::unordered_map<Vec3u, size_t, Hash_Vec3u> vertexIndices;

        LoadLine loadLine;
        ParsePolygonVertex parsePolygonVertex;
        MakeVertex makeVertex;


        void loadLine_v(std::string& type, std::istringstream& str) {
            if (type == "v") v.emplace_back(parseV3(str));
            else if (type == "f") {
                while (str.rdbuf()->in_avail()) {
                    Vec3u vertex = parsePolygonVertex(str) - 1;
                    if (vertexIndices.find(vertex) == vertexIndices.end())
                        vertexIndices[vertex] = vertexIndices.size();
                    polygon.emplace_back(vertexIndices.at(vertex));
                }
                for (size_t i = 1; i < polygon.size() - 1; ++i)
                    f_out.emplace_back(Vec3u{ polygon[0], polygon[i], polygon[i + 1] });
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


        Vertex makeVertex_v(const Vec3u& i) const {
            return Vertex(v[i[0]]);
        }

        Vertex makeVertex_vt(const Vec3u& i) const {
            return Vertex(v[i[0]], vt[i[1]]);
        }

        Vertex makeVertex_vn(const Vec3u& i) const {
            return Vertex(v[i[0]], vn[i[2]]);
        }

        Vertex makeVertex_vtn(const Vec3u& i) const {
            return Vertex(v[i[0]], vt[i[1]], vn[i[2]]);
        }


    public:
        ObjLoader(
            const char* filename,
            std::vector<Vertex>& v_out,
            std::vector<Vec3u>& f_out,
            bool loadvt = false,
            bool loadvn = false
        ) :
            v_out(v_out),
            f_out(f_out),
            fCount(0),
            vCount(0),
            vtCount(0),
            vnCount(0),
            loadLine(nullptr),
            parsePolygonVertex(nullptr),
            makeVertex(nullptr)
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
            f_out.reserve(fCount);
            vertexIndices.reserve(vCount + vtCount + vnCount);

            bool vtGood = (loadvt && vtCount && (
                std::is_base_of_v<Vertex_Mesh_T,  Vertex> ||
                std::is_base_of_v<Vertex_Mesh_TN, Vertex>
            ));
            bool vnGood = (loadvn && vnCount && (
                std::is_base_of_v<Vertex_Mesh_N,  Vertex> ||
                std::is_base_of_v<Vertex_Mesh_TN, Vertex>
            ));

            if (vtGood) {
                vt.reserve(vtCount);
                if (vnGood) {
                    vn.reserve(vnCount);
                    loadLine = loadLine_vtn;
                    makeVertex = makeVertex_vtn;
                } else {
                    loadLine = loadLine_vt;
                    makeVertex = makeVertex_vt;
                }
            } else if (vnGood) {
                vn.reserve(vnCount);
                loadLine = loadLine_vn;
                makeVertex = makeVertex_vn;
            } else {
                loadLine = loadLine_v;
                makeVertex = makeVertex_v;
            }

            file.clear();
            file.seekg(0, std::ios::beg);

            while (std::getline(file, line)) {
                std::istringstream str(std::move(line));
                str >> line;
                (this->*loadLine)(line, str);
            }

            v_out = std::vector<Vertex>(vertexIndices.size());
            for (auto& [key, value] : vertexIndices) {
                v_out[value] = (this->*makeVertex)(key);
            }
        }
    };

}
