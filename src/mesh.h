#pragma once

#include <string>
#include <vector>
#include "vec.h"

namespace ramiel {

    class Mesh {
        std::vector<Vec3f> v_pos;
		std::vector<Vec3f> v_normal;
		std::vector<Vec2f> v_txt;

		std::vector<Vec3u> tri;
		std::vector<Vec3u> tri_txt;

    public:
        Mesh(std::string filename, Vec3f pos = vec3f_0, Vec3f rot = vec3f_0);

        void getVPos(std::vector<Vec3f>& v_pos, Vec3f pos = vec3f_0, Vec3f rot = vec3f_0) const;
        void getVNormal(std::vector<Vec3f>& v_normal, Vec3f rot) const;

        const std::vector<Vec3f>& getVPos() const;
        const std::vector<Vec3f>& getVNormal() const;
        const std::vector<Vec2f>& getVTxt() const;
        const std::vector<Vec3u>& getTri() const;
        const std::vector<Vec3u>& getTriTxt() const;
    };

}
