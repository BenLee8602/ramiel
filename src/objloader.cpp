#include "objloader.h"

namespace ramiel {

    Vec2f parseV2(std::istream& str) {
        Vec2f v;
        str >> v[0] >> v[1];
        return v;
    }

    Vec3f parseV3(std::istream& str) {
        Vec3f v;
        str >> v[0] >> v[1] >> v[2];
        return v;
    }


    Vec3u parsePolygonVertex_v(std::istream& str) {
        Vec3u v = vec3u_0;
        str >> v[0];
        return v;
    }

    Vec3u parsePolygonVertex_vt(std::istream& str) {
        char junk;
        Vec3u v = vec3u_0;
        str >> v[0] >> junk >> v[1];
        return v;
    }
    
    Vec3u parsePolygonVertex_vn(std::istream& str) {
        char junk;
        Vec3u v = vec3u_0;
        str >> v[0] >> junk >> junk >> v[1];
        return v;
    }
    
    Vec3u parsePolygonVertex_vtn(std::istream& str) {
        char junk;
        Vec3u v = vec3u_0;
        str >> v[0] >> junk >> v[1] >> junk >> v[2];
        return v;
    }

}
