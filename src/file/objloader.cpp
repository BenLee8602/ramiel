#include <fstream>
#include <sstream>
#include <unordered_map>
#include "objloader.h"

using namespace ramiel;
using FaceVtx = ObjData::FaceVtx;
using Face = ObjData::Face;

namespace {

    struct Hash_FaceVtx {
        size_t operator()(const FaceVtx& key) const {
            return key[0] << 16 + key[1] << 8 + key[2];
        }
    };


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

    FaceVtx parseFaceVtx(std::string& vstr) {
        std::istringstream str(std::move(vstr));
        std::string token;
        FaceVtx v{};
        for (size_t i = 0; i < 3; ++i) {
            if (!std::getline(str, token, '/')) return v;
            if (!token.length()) continue;
            v[i] = std::stoi(token);
        }
        return v;
    }


    void make_v_impl(
        const Vec3f& v,
        const Vec2f&,
        const Vec3f&,
        float* out
    ) {
        *reinterpret_cast<Vec3f*>(out) = v;
    }

    void make_vt_impl(
        const Vec3f& v,
        const Vec2f& vt,
        const Vec3f&,
        float* out
    ) {
        *reinterpret_cast<Vec3f*>(out) = v;
        *reinterpret_cast<Vec2f*>(out + 3) = vt;
    }

    void make_vn_impl(
        const Vec3f& v,
        const Vec2f&,
        const Vec3f& vn,
        float* out
    ) {
        *reinterpret_cast<Vec3f*>(out) = v;
        *reinterpret_cast<Vec3f*>(out + 3) = vn;
    }

    void make_vtn_impl(
        const Vec3f& v,
        const Vec2f& vt,
        const Vec3f& vn,
        float* out
    ) {
        *reinterpret_cast<Vec3f*>(out) = v;
        *reinterpret_cast<Vec2f*>(out + 3) = vt;
        *reinterpret_cast<Vec3f*>(out + 5) = vn;
    }

}

namespace ramiel {

    ObjData loadObj(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return ObjData();

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

        std::vector<Face> f;
        std::vector<Vec3f> v;
        std::vector<Vec2f> vt;
        std::vector<Vec3f> vn;

        f.reserve(fCount);
        v.reserve(vCount + 1);
        vt.reserve(vtCount + 1);
        vn.reserve(vnCount + 1);

        v.emplace_back(Vec3f{});
        vt.emplace_back(Vec2f{});
        vn.emplace_back(Vec3f{});

        file.clear();
        file.seekg(0, std::ios::beg);

        std::vector<FaceVtx> polygon;
        while (std::getline(file, token)) {
            std::istringstream str(std::move(token));
            str >> token;
            if (token == "f") {
                while (str >> token) {
                    polygon.emplace_back(parseFaceVtx(token));
                }
                for (size_t i = 1; i < polygon.size() - 1; ++i) {
                    f.emplace_back(Face{ polygon[0], polygon[i], polygon[i + 1] });
                }
                polygon.clear();
            }
            else if (token == "v") v.emplace_back(parseVec3(str));
            else if (token == "vt") vt.emplace_back(parseVec2(str));
            else if (token == "vn") vn.emplace_back(parseVec3(str));
        }

        return {
            std::move(f),
            std::move(v),
            std::move(vt),
            std::move(vn)
        };
    }


    VertexBuffer makeVertexBuffer(const ObjData& data) {
        bool has_vt = data.vt.size() > 1;
        bool has_vn = data.vn.size() > 1;

        size_t vertexSize = 3;
        VertexFactory vertexFactory = make_v();

        if (has_vt && has_vn) {
            vertexSize = 8;
            vertexFactory = make_vtn();
        }
        else if (has_vt) {
            vertexSize = 5;
            vertexFactory = make_vt();
        }
        else if (has_vn) {
            vertexSize = 6;
            vertexFactory = make_vn();
        }

        return makeVertexBuffer(data, vertexSize, vertexFactory);
    }


    VertexBuffer makeVertexBuffer(const ObjData& data, size_t vertexSize, VertexFactory vertexFactory) {
        std::vector<uint32_t> fOut;
        std::unordered_map<FaceVtx, size_t, Hash_FaceVtx> vertexMap;

        fOut.reserve(data.f.size() * 3);
        vertexMap.reserve(data.f.size() * 3);

        for (auto& f : data.f) {
            for (auto& v : f) {
                auto vertex = vertexMap.find(v);
                if (vertex == vertexMap.end()) {
                    vertexMap.insert({ v, vertexMap.size() });
                }
                fOut.emplace_back(vertexMap[v]);
            }
        }

        std::vector<float> vOut(vertexSize * vertexMap.size());

        for (auto& [v, i] : vertexMap) {
            vertexFactory(
                data.v[v[0]],
                data.vt[v[1]],
                data.vn[v[2]],
                vOut.data() + vertexSize * i
            );
        }

        return {
            std::move(fOut),
            std::move(vOut)
        };
    }


    VertexFactory make_v() {
        return make_v_impl;
    }

    VertexFactory make_vt() {
        return make_vt_impl;
    }

    VertexFactory make_vn() {
        return make_vn_impl;
    }

    VertexFactory make_vtn() {
        return make_vtn_impl;
    }
    
}
