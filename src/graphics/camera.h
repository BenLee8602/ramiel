#pragma once

#include <vector>
#include <ramiel/math.h>

namespace ramiel {

    const Vec3f& getPos();
    const Vec3f& getRot();

    void setPos(const Vec3f& pos);
    void setRot(const Vec3f& rot);

    const Vec3f& getBackgroundColor();
    void setBackgroundColor(const Vec3f& color);

    float getZ0();
    float getZ1();

    void setZ0(float z0);
    void setZ1(float z1);
    
    size_t getBufferSize();
    const Vec2u& getRes();
    void setRes(Vec2u size);

    float getFov();
    void setFov(float fov);

    void resetBuffers();

    Vec3f getCameraCoord(const Vec3f& in);
    Vec2f getScreenCoord(const Vec3f& in);

    typedef std::vector<Vec3f>::iterator ColorBufferIterator;
    typedef std::vector<float>::iterator DepthBufferIterator;
    ColorBufferIterator getColorBuffer();
    DepthBufferIterator getDepthBuffer();

    void clampColorBuffer();

    void getFrameDEC(int* frame);
    void getFrameRGB(uint8_t* frame);

}
