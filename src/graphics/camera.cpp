#include <cmath>
#include "camera.h"

namespace ramiel {

    size_t Camera::getBufferSize() const {
        return bufferSize;
    }

    const Vec2u& Camera::getRes() const {
        return res;
    }

    void Camera::setRes(Vec2u newSize) {
        res = newSize;
        halfRes = res / 2.0f;
        bufferSize = res[X] * res[Y];
        focalLength = halfRes[X] / std::tan(fov / 2.0f);
        color = std::vector<Vec3f>(bufferSize);
        depth = std::vector<float>(bufferSize);
    }


    float Camera::getFov() const {
        return fov;
    }

    void Camera::setFov(float deg) {
        fov = deg * 0.01745f;
        focalLength = halfRes[X] / std::tan(fov / 2.0f);
    }


    void Camera::resetBuffers() {
        std::fill(color.begin(), color.end(), backgroundColor);
        std::fill(depth.begin(), depth.end(), zfar);

        // temp
        transform = matmat(translate(-pos), rotate(rot));
    }


    Vec3f Camera::getCameraCoord(const Vec3f& in) const {
        Vec4f in4 = { in[X], in[Y], in[Z], 1 };
        Vec4f out4 = matvec(transform, in4);
        return { out4[X], out4[Y], out4[Z] };
    }


    Vec2f Camera::getScreenCoord(const Vec3f& in) const {
        if (in[Z] == 0.0f) return Vec2f();
        Vec2f out = Vec2f();
        out[X] = std::floor(in[X] * focalLength / in[Z] + halfRes[X]);
        out[Y] = std::floor(in[Y] * focalLength / in[Z] + halfRes[Y]);
        return out;
    }


    Camera::ColorBufferIterator Camera::getColorBuffer() {
        return color.begin();
    }

    Camera::DepthBufferIterator Camera::getDepthBuffer() {
        return depth.begin();
    }


    void Camera::clampColorBuffer() {
        for (auto& c : color) c = min(c, 255.0f);
    }


    void Camera::getFrameDEC(int* frame) const {
        for (auto& c : color)
            *frame++ = ((int)c[R] << 16) + ((int)c[G] << 8) + (int)c[B];
    }

    void Camera::getFrameRGB(uint8_t* frame) const {
        for (auto& c : color) {
            *frame++ = c[R];
            *frame++ = c[G];
            *frame++ = c[B];
        }
    }

}
