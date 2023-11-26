#pragma once

#include <string>
#include <vector>
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

    bool loadObj(
        const char* filename,
        std::vector<Vec3u>& f,
        std::vector<Vec3f>& v,
        std::vector<Vec2f>& vt,
        std::vector<Vec3f>& vn,
        std::unordered_map<Vec3u, size_t, Hash_Vec3u>& vertexIndices
    );


    template<class Vertex>
    bool getObj(const char* filename, std::vector<Vertex>& v_out, std::vector<Vec3u>& f_out) {
        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;
        std::unordered_map<Vec3u, size_t, Hash_Vec3u> vertexIndices;

        if (!loadObj(filename, f_out, v, vt, vn, vertexIndices)) return false;

        v_out = std::vector<Vertex>(vertexIndices.size());
        for (auto& [key, value] : vertexIndices) {
            v_out[value] = Vertex(v[key[0]], vt[key[1]], vn[key[2]]);
        }
        return true;
    }

}
