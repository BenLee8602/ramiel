#pragma once

#include <string>
#include <vector>
#include "vec.h"
#include "rotation.h"

namespace ramiel {

    class Mesh {
        std::vector<Vec3f> v_pos;
		std::vector<Vec3f> v_normal;
		std::vector<Vec2f> v_txt;
		std::vector<Vec3u> tri;
    public:
        Mesh(
            std::string filename,
            float scale = 1.0f,
            Vec3f pos = vec3f_0,
            Rotation rot = Rotation()
        );
        const std::vector<Vec3f>& getVPos() const;
        const std::vector<Vec3f>& getVNormal() const;
        const std::vector<Vec2f>& getVTxt() const;
        const std::vector<Vec3u>& getTri() const;
        const std::vector<Vec3u>& getTriTxt() const;
    };

}
