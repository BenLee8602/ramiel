#include <cmath>
#include "mesh.h"
#include "objloader.h"

namespace ramiel {

    Mesh::Mesh(std::string filename, float scale, Vec3f pos, Rotation rot) {
        std::vector<Vec3u> tri_txt;
        std::vector<Vec2f> v_txt_temp;

        // allocate memory
		size_t n_v, n_vt, n_f;
		objloader::count(filename, n_v, n_vt, n_f);
		v_pos.reserve(n_v);
		v_normal = std::vector<Vec3f>(n_v);
		tri.reserve(n_f);
		if (n_vt) {
			v_txt_temp.reserve(n_vt);
			tri_txt.reserve(n_f);
            v_txt.reserve(n_v);
		}
		
		// get obj data
		objloader::load(filename, v_pos, v_txt_temp, tri, tri_txt);

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
        for (size_t i = 0; i < n_v; ++i) {
            v_pos[i] = rot.rotate(v_pos[i]) * scale + pos;
            v_normal[i] = rot.rotate(v_normal[i]);
        }

        // make texture coords parallel with pos+normal
        for (size_t i = 0; i < n_f; ++i) {
            for (size_t j = 0; j < 3; ++j) {
                if (!v_txt[tri[i][j]]) {
                    v_txt[tri[i][j]] = v_txt_temp[tri_txt[i][j]];
                }
            }
        }
    }


    const std::vector<Vec3f>& Mesh::getVPos()    const { return v_pos; }
    const std::vector<Vec3f>& Mesh::getVNormal() const { return v_normal; }
    const std::vector<Vec2f>& Mesh::getVTxt()    const { return v_txt; }
    const std::vector<Vec3u>& Mesh::getTri()     const { return tri; }

}
