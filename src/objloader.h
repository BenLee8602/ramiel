#pragma once

#include <string>
#include <vector>
#include "vec.h"

namespace ramiel::objloader {

    std::vector<size_t> splitv(const std::string& in);
    void count(
        std::string filename,
        size_t& v, size_t& vt, size_t& f
    );
    void load(
        std::string filename,
        std::vector<Vec3f>& v,
        std::vector<Vec2f>& v_txt,
        std::vector<Vec3u>& tri,
        std::vector<Vec3u>& tri_txt
    );

}
