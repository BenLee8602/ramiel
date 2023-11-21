#pragma once

#include <unordered_map>
#include <fstream>

#include <ramiel/math.h>
#include <ramiel/file.h>
#include <ramiel/graphics.h>
#include <ramiel/physics.h>

namespace ramiel {

    class Scene {
    public:
        Camera camera;
        Vec3f ambientLight = vec3f_0;

    private:
        std::unordered_map<std::string, MeshBase*> meshes;
        std::unordered_map<std::string, Texture*>  textures;

    private:
        std::vector<EntityBase*> entities;
        std::vector<Light*> lights;
        std::vector<Effect*> effects;
    
    private:
        std::vector<Dynamics*> dynamicObjects;
        std::vector<Collider*> colliders;

    public:
        template<class Vertex>
        bool loadMesh(const char* filename, const char* meshname, bool loadvt = false, bool loadvn = false) {
            if (!std::ifstream(filename).good()) return false;
            if (meshes[meshname]) return false;
            std::vector<Vertex> vertices;
            std::vector<Vec3u> triangles;
            getObj(filename, vertices, triangles);
            meshes[meshname] = new Mesh<Vertex>(std::move(triangles), std::move(vertices));
            return true;
        }
        bool loadTexture(const char* filename, const char* textureName);
        bool loadNormalMap(const char* filename, const char* normalMapName);
        const Texture* getTexture(const char* textureName) const;

        template<class Vertex, class VertexShader, class PixelShader>
        bool addEntity(
            const char* meshName,
            VertexShader vertexShader,
            PixelShader pixelShader
        ) {
            Mesh<Vertex>* mesh = dynamic_cast<Mesh<Vertex>*>(meshes[meshName]);
            if (!mesh) return false;
            entities.push_back(new Entity(*mesh, vertexShader, pixelShader));
            return true;
        }
        void addLight(Light* light);
        void addEffect(Effect* effect);

        LightingList getLightingList() const;
        LightingListSpecular getLightingList(uint16_t specularExponent, float specularIntensity) const;

        void renderFrame();


        bool addDynamicObject(Dynamics* dynamicObject);
        bool removeDynamicObject(Dynamics* dynamicObject);

        bool addCollider(Collider* collider);
        bool removeCollider(Collider* collider);

        void simulatePhysics(float dtime);


        ~Scene();
    };

}
