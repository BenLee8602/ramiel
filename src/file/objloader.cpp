#include <fstream>
#include <sstream>
#include "objloader.h"

namespace ramiel {

    Vec2f parseVec2(std::istream& str) {
        Vec2f v;
        str >> v[0] >> v[1];
        return v;
    }

    Vec3f parseVec3(std::istream& str) {
        Vec3f v;
        str >> v[0] >> v[1] >> v[2];
        return v;
    }

    Vec3u parsePolygonVertex(std::string& vstr) {
        std::istringstream str(std::move(vstr));
        std::string token;
        Vec3u v{};
        for (size_t i = 0; i < 3; ++i) {
            if (!std::getline(str, token, '/')) return v;
            if (!token.length()) continue;
            v[i] = std::stoi(token);
        }
        return v;
    }


    bool loadObj(
        const std::string& filename,
        std::vector<Vec3u>& f,
        std::vector<Vec3f>& v,
        std::vector<Vec2f>& vt,
        std::vector<Vec3f>& vn,
        std::unordered_map<Vec3u, size_t, Hash_Vec3u>& vertexIndices
    ) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        size_t fCount = 0;
        size_t vCount = 0;
        size_t vtCount = 0;
        size_t vnCount = 0;

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
        f.reserve(fCount);
        vertexIndices.reserve(vCount + vtCount + vnCount);

        v.emplace_back(Vec3f{});
        vt.emplace_back(Vec2f{});
        vn.emplace_back(Vec3f{});

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<uint32_t> polygon;
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
                    f.emplace_back(Vec3u{ polygon[0], polygon[i], polygon[i + 1] });
                }
                polygon.clear();
            }
            else if (token == "v") v.emplace_back(parseVec3(str));
            else if (token == "vt") vt.emplace_back(parseVec2(str));
            else if (token == "vn") vn.emplace_back(parseVec3(str));
        }

        return true;
    }
    
}
