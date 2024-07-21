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

    float fov = 1.57f;
    float focalLength = 1.0f;
    float z0 = 0.2f;
    float z1 = 1000.0f;

    Mat4x4f transform = id<float, 4>();

    Vec3f backgroundColor = Vec3f();
    
}

namespace ramiel {

    const Vec2u& getRes() {
        return res;
    }

    void setRes(Vec2u newSize) {
        res = newSize;
        halfRes = res / 2.0f;
        bufferSize = res[X] * res[Y];
        focalLength = halfRes[X] / std::tan(fov / 2.0f);
        color = std::vector<Vec3f>(bufferSize);
        depth = std::vector<float>(bufferSize);
    }

    size_t getBufferSize() {
        return bufferSize;
    }


    ColorBufferIterator getColorBuffer() {
        return color.begin();
    }

    DepthBufferIterator getDepthBuffer() {
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
    }

    void setRot(const Vec3f& rot) {
        ::rot = rot;
    }


    float getFov() {
        return fov;
    }

    float getZ0() {
        return z0;
    }

    float getZ1() {
        return z1;
    }


    void setFov(float deg) {
        fov = deg * 0.01745f;
        focalLength = halfRes[X] / std::tan(fov / 2.0f);
    }

    void setZ0(float z0) {
        ::z0 = z0;
    }

    void setZ1(float z1) {
        ::z1 = z1;
    }


    Vec3f getCameraCoord(const Vec3f& in) {
        Vec4f in4 = { in[X], in[Y], in[Z], 1 };
        Vec4f out4 = matvec(transform, in4);
        return { out4[X], out4[Y], out4[Z] };
    }

    Vec2f getScreenCoord(const Vec3f& in) {
        if (in[Z] == 0.0f) return Vec2f();
        Vec2f out = Vec2f();
        out[X] = std::floor(in[X] * focalLength / in[Z] + halfRes[X]);
        out[Y] = std::floor(in[Y] * focalLength / in[Z] + halfRes[Y]);
        return out;
    }


    const Vec3f& getBackgroundColor() {
        return backgroundColor;
    }

    void setBackgroundColor(const Vec3f& color) {
        backgroundColor = color;
    }


    void resetBuffers() {
        std::fill(color.begin(), color.end(), backgroundColor);
        std::fill(depth.begin(), depth.end(), z1);

        // temp
        transform = matmat(translate(-pos), rotate(rot));
    }

    void clampColorBuffer() {
        for (auto& c : color) c = min(c, 255.0f);
    }


    void getFrameDEC(int* frame) {
        for (auto& c : color)
            *frame++ = ((int)c[R] << 16) + ((int)c[G] << 8) + (int)c[B];
    }

    void getFrameRGB(uint8_t* frame) {
        for (auto& c : color) {
            *frame++ = c[R];
            *frame++ = c[G];
            *frame++ = c[B];
        }
    }

}
