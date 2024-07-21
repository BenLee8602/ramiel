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

    void loadMesh(const char* meshName, MeshBase* mesh);
    template<class Vertex>
    void loadMesh(const char* filename, const char* meshname, bool loadvt = false, bool loadvn = false) {
        std::vector<Vertex> vertices;
        std::vector<Vec3u> triangles;
        getObj(filename, vertices, triangles);
        MeshBase* mesh = new Mesh<Vertex>(std::move(triangles), std::move(vertices));
        loadMesh(meshname, mesh);
    }
    MeshBase* getMesh(const char* meshName);

    bool loadTexture(const char* filename, const char* textureName, Mat4x4f format);
    Texture* getTexture(const char* textureName);

    void addEntity(EntityBase* entity);
    template<class Vertex, class VertexShader, class PixelShader>
    void addEntity(
        const char* meshName,
        VertexShader vertexShader,
        PixelShader pixelShader
    ) {
        Mesh<Vertex>* mesh = static_cast<Mesh<Vertex>*>(getMesh(meshName));
        EntityBase* entity = new Entity(*mesh, vertexShader, pixelShader);
        addEntity(entity);
    }

    const Vec3f& getAmbientLight();
    void setAmbientLight(const Vec3f& color);
    void addLight(Light* light);
    LightingList getLightingList();
    LightingListSpecular getLightingList(uint16_t specularExponent, float specularIntensity);

    void addEffect(Effect* effect);

    void renderFrame();


    bool addDynamicObject(Kinematics* dynamicObject);
    bool removeDynamicObject(Kinematics* dynamicObject);

    bool addCollider(Collider* collider);
    bool removeCollider(Collider* collider);

    void simulatePhysics(float dtime);


    void destroy();

}
