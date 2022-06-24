#include <cmath>
#include "mesh.h"
#include "objloader.h"

namespace ramiel {

    inline void rotate(Vec3f& v, const Vec3f& sin, const Vec3f& cos) {
        Vec3f out = v;

        // z rot
        out[X] = v[X] * cos[Z] + v[Y] * -sin[Z];
        out[Y] = v[X] * sin[Z] + v[Y] * cos[Z];
        v = out;

        // y rot
        out[X] = v[X] * cos[Y] + v[Z] * sin[Y];
        out[Z] = v[X] * -sin[Y] + v[Z] * cos[Y];
        v = out;

        // x rot
        out[Y] = v[Y] * cos[X] + v[Z] * -sin[X];
        out[Z] = v[Y] * sin[X] + v[Z] * cos[X];
        v = out;
    }

    Mesh::Mesh(std::string filename, float scale, Vec3f pos, Vec3f rot) {
        // allocate memory
		size_t n_v, n_vt, n_f;
		objloader::count(filename, n_v, n_vt, n_f);
		v_pos.reserve(n_v);
		v_normal = std::vector<Vec3f>(n_v);
		tri.reserve(n_f);
		if (n_vt) {
			v_txt.reserve(n_vt);
			tri_txt.reserve(n_f);
		}
		
		// get obj data
		objloader::load(filename, v_pos, v_txt, tri, tri_txt);

		// calc vertex normals
		std::vector<Vec3f> tri_normals(tri.size());
		for (auto& t : tri) {
			Vec3f v1 = v_pos[t[1]] - v_pos[t[0]];
			Vec3f v2 = v_pos[t[2]] - v_pos[t[0]];
			Vec3f normal = crossProduct(v1, v2);
			for (int i = 0; i < 3; ++i) v_normal[t[i]] += normal;
		}
		for (auto& n : v_normal) n = getNormalized(n);

        // transform
        if (scale != 1.0f) for (auto& v : v_pos) v *= scale;
        if (rot) {
            Vec3f sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
            Vec3f cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
            for (size_t i = 0; i < v_pos.size(); ++i) {
                rotate(v_pos[i], sin, cos);
                rotate(v_normal[i], sin, cos);
            }
        }
		if (pos) for (auto& v : v_pos) v += pos;
    }


    void Mesh::getVPos(std::vector<Vec3f>& v_pos, float scale, const Vec3f& pos, const Vec3f& rot) const {
        v_pos = this->v_pos;
        if (scale != 1.0f) for (auto& v : v_pos) v *= scale;
        if (rot) {
            Vec3f sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
            Vec3f cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
            for (auto& v : v_pos) rotate(v, sin, cos);
        }
        if (pos) for (auto& v : v_pos) v += pos;
    }


    void Mesh::getVNormal(std::vector<Vec3f>& v_normal, Vec3f rot) const {
        v_normal = this->v_normal;
        if (rot) {
            Vec3f sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
            Vec3f cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
            for (auto& v : v_normal) rotate(v, sin, cos);
        }
    }


    const std::vector<Vec3f>& Mesh::getVPos()    const { return v_pos; }
    const std::vector<Vec3f>& Mesh::getVNormal() const { return v_normal; }
    const std::vector<Vec2f>& Mesh::getVTxt()    const { return v_txt; }
    const std::vector<Vec3u>& Mesh::getTri()     const { return tri; }
    const std::vector<Vec3u>& Mesh::getTriTxt()  const { return tri_txt; }

}
