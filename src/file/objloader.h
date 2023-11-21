#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

#include <ramiel/math.h>


namespace ramiel {

    struct Hash_Vec3u {
        size_t operator()(const Vec3u& key) const {
            return key[0] << 20 + key[1] << 10 + key[2];
        }
    };

    Vec2f parseVec2(std::istream& str);
    Vec3f parseVec3(std::istream& str);
    Vec3u parsePolygonVertex(std::string& str);


    template<class Vertex>
    void loadObj(const char* filename, std::vector<Vertex>& v_out, std::vector<Vec3u>& f_out) {
        std::ifstream file(filename);
        if (!file.is_open()) return;

        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;

        size_t fCount = 0;
        size_t vCount = 0;
        size_t vtCount = 0;
        size_t vnCount = 0;

        std::vector<uint32_t> polygon;
        std::unordered_map<Vec3u, size_t, Hash_Vec3u> vertexIndices;

        std::string token;
        while (std::getline(file, token)) {
            std::istringstream str(std::move(token));
            str >> token;
            if (token == "v") ++vCount;
            else if (token == "f") {
                size_t nVertices = 0;
                while (str >> token) ++nVertices;
                fCount += nVertices - 2;
            }
            else if (token == "vt") ++vtCount;
            else if (token == "vn") ++vnCount;
        }

        v.reserve(vCount + 1);
        vt.reserve(vtCount + 1);
        vn.reserve(vnCount + 1);
        f_out.reserve(fCount);
        vertexIndices.reserve(vCount + vtCount + vnCount);

        v.emplace_back(vec3f_0);
        vt.emplace_back(vec2f_0);
        vn.emplace_back(vec3f_0);

        file.clear();
        file.seekg(0, std::ios::beg);

        while (std::getline(file, token)) {
            std::istringstream str(std::move(token));
            str >> token;
            if (token == "f") {
                while (str >> token) {
                    Vec3u vertex = parsePolygonVertex(token);
                    if (vertexIndices.find(vertex) == vertexIndices.end()) {
                        vertexIndices[vertex] = vertexIndices.size();
                    }
                    polygon.emplace_back(vertexIndices.at(vertex));
                }
                for (size_t i = 1; i < polygon.size() - 1; ++i) {
                    f_out.emplace_back(Vec3u{ polygon[0], polygon[i], polygon[i + 1] });
                }
                polygon.clear();
            }
            else if (token == "v") v.emplace_back(parseVec3(str));
            else if (token == "vt") vt.emplace_back(parseVec2(str));
            else if (token == "vn") vn.emplace_back(parseVec3(str));
        }

        v_out = std::vector<Vertex>(vertexIndices.size());
        for (auto& [key, value] : vertexIndices) {
            v_out[value] = Vertex(v[key[0]], vt[key[1]], vn[key[2]]);
        }
    }

}
