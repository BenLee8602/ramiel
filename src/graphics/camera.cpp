#include <cassert>
#include <cmath>
#include "camera.h"
using namespace ramiel;

namespace {

    Vec2u res = Vec2u();
    Vec2f halfRes = Vec2f();
    size_t bufferSize = 0;
    
    std::vector<Vec3f> color;
    std::vector<float> depth;

    Vec3f pos = Vec3f();
    Vec3f rot = Vec3f();

    float fov = 1.570796f;
    float focalLen = 1.0f;
    float z0 = 0.2f;
    float z1 = 1000.0f;

    Mat4x4f cameraTransform = id<float, 4>();


    void calcCameraTransform() {
        cameraTransform = matmat(translate(-pos), rotate(rot));
    }
    
}

namespace ramiel {

    const Vec2u& getRes() {
        return res;
    }

    void setRes(Vec2u newSize) {
        res = newSize;
        halfRes = res / 2.0f;
        bufferSize = res[X] * res[Y];
        focalLen = halfRes[X] / std::tan(fov / 2.0f);
        color = std::vector<Vec3f>(bufferSize);
        depth = std::vector<float>(bufferSize);
    }

    size_t getBufferSize() {
        return bufferSize;
    }


    ColorIt getColorBuffer() {
        return color.begin();
    }

    DepthIt getDepthBuffer() {
        return depth.begin();
    }


    const Vec3f& getPos() {
        return pos;
    }

    const Vec3f& getRot() {
        return rot;
    }


    void setPos(const Vec3f& pos) {
        ::pos = pos;
        calcCameraTransform();
    }

    void setRot(const Vec3f& rot) {
        ::rot = rot;
        calcCameraTransform();
    }

    void setPosRot(const Vec3f& pos, const Vec3f& rot) {
        ::pos = pos;
        ::rot = rot;
        calcCameraTransform();
    }


    float getFov() {
        return fov;
    }

    float getFocalLen() {
        return focalLen;
    }

    float getZ0() {
        return z0;
    }

    float getZ1() {
        return z1;
    }


    void setFov(float deg) {
        assert(deg >= 1e-6 && deg <= 180.0f && "invalid fov");
        fov = deg * 0.01745f;
        focalLen = halfRes[X] / std::tan(fov / 2.0f);
    }

    void setFocalLen(float focalLen) {
        assert(focalLen >= 1e-6 && "invalid focal length");
        ::focalLen = std::max(0.0f, focalLen);
        ::fov = 2.0f * std::atan(halfRes[X] / ::focalLen);
    }

    void setZ0(float z0) {
        assert(z0 >= 1e-6 && z0 <= ::z1 && "invalid z0");
        ::z0 = z0;
    }

    void setZ1(float z1) {
        assert(z1 >= ::z0 && "invalid z1");
        ::z1 = z1;
    }


    Vec3f getCameraCoord(const Vec3f& in) {
        Vec4f in4 = { in[X], in[Y], in[Z], 1 };
        Vec4f out4 = matvec(cameraTransform, in4);
        return { out4[X], out4[Y], out4[Z] };
    }

    Vec2f getScreenCoord(const Vec3f& in) {
        if (in[Z] == 0.0f) return Vec2f();
        Vec2f out = Vec2f();
        out[X] = std::floor(in[X] * focalLen / in[Z] + halfRes[X]);
        out[Y] = std::floor(in[Y] * focalLen / in[Z] + halfRes[Y]);
        return out;
    }

}
