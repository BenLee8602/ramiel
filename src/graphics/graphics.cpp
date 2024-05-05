#include "graphics.h"
#include <ramiel/math.h>

namespace {

    using namespace ramiel;

    // output resolution
    Vec2u res = Vec2u();
    size_t bufferSize = 0U;

    // buffers
    std::vector<Vec3f> color;
    std::vector<float> depth;

    // camera
    Vec3f pos = Vec3f();
    Vec3f rot = Vec3f();

    // view frustum
    float fov;
    float focalLen;
    float zNear;
    float zFar;

    // space transforms
    Mat4x4f cameraSpace;
    Mat3x4f screenSpace;

    void calcCameraMatrix() {
        // should be -rot but this still works somehow, cba to fix
        cameraSpace = matmat(translate(-pos), rotate(rot));
    }

    void calcScreenMatrix() {
        // TODO!!!
    }

}

namespace ramiel {

    Vec2u res() {
        return ::res;
    }


    void res(Vec2u res) {
        ::res = res;
        ::bufferSize = res[X] * res[Y];

        ::color = std::vector<Vec3f>(::bufferSize);
        ::depth = std::vector<float>(::bufferSize);

        // update focal length to keep the same fov
        fov(fov());
    }


    ItColor color() {
        return ::color.begin();
    }

    ItDepth depth() {
        return ::depth.begin();
    }


    Vec3f pos() {
        return ::pos;
    }
    
    Vec3f rot() {
        return ::rot;
    }
    

    void pos(Vec3f pos) {
        ::pos = pos;
        calcCameraMatrix();
    }
    
    void rot(Vec3f rot) {
        ::rot = rot;
        calcCameraMatrix();
    }
    
    void posrot(Vec3f pos, Vec3f rot) {
        ::pos = pos;
        ::rot = rot;
        calcCameraMatrix();
    }
    

    float fov() {
        return ::fov;
    }
    
    float focalLen() {
        return ::focalLen;
    }
    
    float zNear() {
        return ::zNear;
    }
    
    float zFar() {
        return ::zFar;
    }
    

    void fov(float fov) {
        ::fov = std::max(0.0f, fov);
        ::focalLen = (0.5f * ::res[X]) / std::tan(0.5f * ::fov);
        calcScreenMatrix();
    }
    
    void focalLen(float focalLen) {
        ::focalLen = std::max(0.0f, focalLen);
        ::fov = 2.0f * std::atan(0.5f * ::res[X] / ::focalLen);
        calcScreenMatrix();
    }
    
    void zNear(float zNear) {
        ::zNear = std::max(0.0f, zNear);
    }
    
    void zFar(float zFar) {
        ::zFar = std::max(::zNear, zFar);
    }
    

    Vec4f cameraSpace(Vec4f in) {
        return matvec(::cameraSpace, in);
    }
    
    Vec3f screenSpace(Vec4f in) {
        if (in[Z] == 0.0f) return Vec3f();
        Vec3f out = Vec3f();
        out[X] = std::floor(in[X] * ::focalLen / in[Z] + 0.5f * ::res[X]);
        out[Y] = std::floor(in[Y] * ::focalLen / in[Z] + 0.5f * ::res[Y]);
        return out;
        //return matvec(::screenSpace, in);
    }
    

}
