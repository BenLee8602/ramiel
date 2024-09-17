#pragma once

#include <vector>
#include <ramiel/math.h>

namespace ramiel {
    
    const Vec2u& getRes();
    void setRes(Vec2u size);
    size_t getBufferSize();
    float getAspectRatio();

    typedef std::vector<Vec3f>::iterator ColorIt;
    typedef std::vector<float>::iterator DepthIt;
    ColorIt getColorBuffer();
    DepthIt getDepthBuffer();

    const Vec3f& getPos();
    const Vec3f& getRot();

    void setPos(const Vec3f& pos);
    void setRot(const Vec3f& rot);
    void setPosRot(const Vec3f& pos, const Vec3f& rot);

    float getFov();
    float getFocalLen();
    float getZ0();
    float getZ1();

    void setFov(float fov);
    void setFocalLen(float focalLen);
    void setZ0(float z0);
    void setZ1(float z1);

    Vec4f getCameraCoord(const Vec4f& in);
    Vec4f getProjectionCoord(const Vec4f& in);
    Vec4f getScreenCoord(const Vec4f& in);

}
