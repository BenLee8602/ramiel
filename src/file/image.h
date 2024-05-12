#pragma once

#include <vector>
#include <string>
#include <ramiel/math.h>

namespace ramiel {

    constexpr float inv255   = 1.0f / 255.0f;
    constexpr float inv255_2 = 1.0f / 127.5f;

    constexpr Mat4x4f rgb1 = {
        Vec4f{ inv255,  0.0f,   0.0f,  0.0f },
        Vec4f{  0.0f,  inv255,  0.0f,  0.0f },
        Vec4f{  0.0f,   0.0f,  inv255, 0.0f },
        Vec4f{  0.0f,   0.0f,   0.0f,  1.0f }
    };

    constexpr Mat4x4f normalMap = {
        Vec4f{ inv255_2,   0.0f,     0.0f,   -1.0f },
        Vec4f{   0.0f,   inv255_2,   0.0f,   -1.0f },
        Vec4f{   0.0f,     0.0f,   inv255_2, -1.0f },
        Vec4f{   0.0f,     0.0f,     0.0f,    1.0f }
    };

    bool loadImage(
        const std::string& filename,
        Vec2u& res,
        std::vector<Vec3f>& data,
        Mat4x4f format = id<float, 4>()
    );

}
