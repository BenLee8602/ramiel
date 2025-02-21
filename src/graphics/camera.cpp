#include <cassert>
#include <cmath>

#include "camera.h"
using namespace ramiel;

namespace ramiel {

    // definition in triangle.cpp ;)
    void updateViewFrustum();

}

namespace {

    Vec2u res = Vec2u();
    size_t bufferSize = 0;
    float aspectRatio = 0.0f;
    
    std::vector<Vec3f> color;
    std::vector<float> depth;

    Vec3f pos = Vec3f();
    Vec3f rot = Vec3f();

    float fov = 1.570796f;
    float focalLen = 1.0f;
    float z0 = 0.2f;
    float z1 = 1000.0f;

    Mat4x4f cameraTransform = id<float, 4>();
    Mat4x4f projectionTransform = id<float, 4>();
    Mat4x4f screenTransform = id<float, 4>();


    void calcCameraTransform() {
        cameraTransform = matmat(translate(-pos), rotate(rot));
    }

    void calcProjectionTransform() {
        const float f = focalLen;
        const float a = aspectRatio;
        const float z = z1 / (z1 - z0);
        projectionTransform = {
            Vec4f{ f,    0,    0,    0    },
            Vec4f{ 0,  a * f,  0,    0    },
            Vec4f{ 0,    0,    z, -z0 * z },
            Vec4f{ 0,    0,    1,    0    }
        };
        updateViewFrustum();
    }

    void calcScreenTransform() {
        screenTransform = matmat(
            translate(Vec3f{ 1.0f, 1.0f, 0.0f }),
            scale(Vec3f{ res[X] / 2.0f, res[Y] / 2.0f, 1.0f })
        );
    }
    
}

namespace ramiel {

    const Vec2u& getRes() {
        return res;
    }

    void setRes(Vec2u newSize) {
        res = newSize;
        bufferSize = res[X] * res[Y];
        aspectRatio = (float)res[X] / (float)res[Y];
        color = std::vector<Vec3f>(bufferSize);
        depth = std::vector<float>(bufferSize);
        calcProjectionTransform();
        calcScreenTransform();
    }

    size_t getBufferSize() {
        return bufferSize;
    }

    float getAspectRatio() {
        return aspectRatio;
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
        focalLen = 1.0f / std::tan(fov / 2.0f);
        calcProjectionTransform();
    }

    void setFocalLen(float focalLen) {
        assert(focalLen >= 1e-6 && "invalid focal length");
        ::focalLen = std::max(0.0f, focalLen);
        ::fov = 2.0f * std::atan(1.0f / ::focalLen);
        calcProjectionTransform();
    }

    void setZ0(float z0) {
        assert(z0 >= 1e-6 && z0 <= ::z1 && "invalid z0");
        ::z0 = z0;
        calcProjectionTransform();
    }

    void setZ1(float z1) {
        assert(z1 >= ::z0 && "invalid z1");
        ::z1 = z1;
        calcProjectionTransform();
    }


    Vec4f getCameraCoord(const Vec4f& in) {
        return matvec(cameraTransform, in);
    }

    Vec4f getProjectionCoord(const Vec4f& in) {
        return matvec(projectionTransform, in);
    }

    Vec4f getScreenCoord(const Vec4f& in) {
        return matvec(screenTransform, in);
    }

}
