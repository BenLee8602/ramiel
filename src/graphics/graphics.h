#pragma once

#include <vector>
#include <ramiel/math.h>

namespace ramiel {

    Vec2u res();
    void res(Vec2u res);

    typedef std::vector<Vec3f>::iterator ItColor;
    typedef std::vector<float>::iterator ItDepth;
    ItColor color();
    ItDepth depth();

    Vec3f pos();
    Vec3f rot();

    void pos(Vec3f pos);
    void rot(Vec3f rot);
    void posrot(Vec3f pos, Vec3f rot);

    float fov();
    float focalLen();
    float zNear();
    float zFar();

    void fov(float fov);
    void focalLen(float focalLen);
    void zNear(float zNear);
    void zFar(float zFar);

    Vec4f cameraSpace(Vec4f in);
    Vec3f screenSpace(Vec4f in);

}
