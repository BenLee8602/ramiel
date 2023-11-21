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
    
}
