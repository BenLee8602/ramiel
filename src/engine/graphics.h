#pragma once

#include <ramiel/graphics.h>

namespace ramiel {

    class Entity;

    Vec2u getCameraRes();
    void setCameraRes(Vec2u res);

    float getCameraAspectRatio();

    Vec3f getCameraPos();
    void setCameraPos(Vec3f pos);

    Vec3f getCameraRot();
    void setCameraRot(Vec3f rot);

    float getCameraFov();
    void setCameraFov(float fov);

    float getCameraFocalLength();
    void setCameraFocalLength(float focalLength);

    Vec3f getAmbientLightColor();
    void setAmbientLightColor(Vec3f color);

    Vec3f getBackgroundColor();
    void setBackgroundColor(Vec3f color);

    void addGraphicsEntity(Entity* e);
    void removeGraphicsEntity(Entity* e);

    void addGraphicsLight(Light* l);
    void removeGraphicsLight(Light* l);

    void renderFrame();

}
