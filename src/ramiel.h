#pragma once

#include <unordered_map>
#include <fstream>

#include <ramiel/math.h>
#include <ramiel/file.h>
#include <ramiel/data.h>
#include <ramiel/graphics.h>
#include <ramiel/physics.h>

namespace ramiel {

    const Vec3f& getBackgroundColor();
    void setBackgroundColor(const Vec3f& color);

    void addEntity(Entity&& entity);

    const Vec3f& getAmbientLight();
    void setAmbientLight(const Vec3f& color);
    void addLight(Light* light);
    Vec3f getLight(
        const Vec3f& pos,
        const Vec3f& normal,
        unsigned specularExponent = 0,
        float specularIntensiry = 0.0f
    );

    void renderFrame();


    bool addDynamicObject(Kinematics* dynamicObject);
    bool removeDynamicObject(Kinematics* dynamicObject);

    bool addCollider(Collider* collider);
    bool removeCollider(Collider* collider);

    void simulatePhysics(float dtime);


    void destroy();

}
