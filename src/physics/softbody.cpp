#include <cassert>
#include <unordered_set>

#include "softbody.h"
#include "entity.h"
#include "constraint.h"
#include "physics.h"
using namespace ramiel;

namespace {

    using Edge = std::pair<Particle*, Particle*>;
    
    class HashEdge {
    public:
        size_t operator()(const Edge& key) const {
            size_t hash1 = reinterpret_cast<size_t>(key.first);
            size_t hash2 = reinterpret_cast<size_t>(key.second);
            return hash1 ^ (hash2 << 1);
        }
    };

    using EdgeMap = std::unordered_set<Edge, HashEdge>;

}

namespace ramiel {

    SoftBody::SoftBody(const Mesh& mesh, float k)
        : mesh(std::make_shared<Mesh>(mesh))
        , enabled(false)
    {
        assert(mesh.getAttrOutType().size() == 1);
        assert(mesh.getAttrOutType()[0] == 3);

        entities.reserve(mesh.getVertexCount());
        for (auto v = mesh.vtxBegin(); v < mesh.vtxEnd(); v += mesh.getVertexSize()) {
            auto pos = reinterpret_cast<const Vec3f*>(v);
            auto c = new Particle(*pos, Vec3f{}, 1.0f);
            entities.emplace_back(c);
        }

        EdgeMap edgeMap;
        edgeMap.reserve(mesh.getTriangleCount() * 3);
        for (auto t = mesh.triBegin(); t < mesh.triEnd(); t += 3) {
            for (uint32_t i = 0; i < 3; i++) {
                Particle* v0 = entities[t[i]];
                Particle* v1 = entities[t[(i + 1) % 3]];

                if (edgeMap.find(std::make_pair(v0, v1)) != edgeMap.end()) continue;
                if (edgeMap.find(std::make_pair(v1, v0)) != edgeMap.end()) continue;
                edgeMap.insert(std::make_pair(v0, v1));
            }
        }

        constraints.reserve(edgeMap.size() + entities.size());
        for (auto& edge : edgeMap) {
            float l0 = mag(edge.first->pos - edge.second->pos);
            auto c = new DistanceConstraint(l0, k, edge.first, edge.second);
            constraints.emplace_back(c);
        }

        // temp: add ground constraints
        for (auto e : entities) {
            auto c = new GroundConstraint(e);
            constraints.emplace_back(c);
        }
    }


    SoftBody::~SoftBody() {
        disable();
        
        for (auto c : constraints) delete c;
        for (auto e : entities) delete e;
    }


    SoftBody::operator bool() const {
        return enabled;
    }


    void SoftBody::enable() {
        if (enabled) return;
        enabled = true;

        for (auto c : constraints) addConstraint(c);
        for (auto e : entities) addEntity(e);
    }


    void SoftBody::disable() {
        if (!enabled) return;
        enabled = false;

        for (auto c : constraints) removeConstraint(c);
        for (auto e : entities) removeEntity(e);
    }


    std::shared_ptr<Mesh> SoftBody::getMesh() {
        return mesh;
    }


    void SoftBody::updateMesh() {
        const size_t vtxSize = mesh->getVertexSize();
        float* vtx = mesh->vtxBegin();
        
        for (size_t i = 0; i < entities.size(); i++) {
            Vec3f* meshPos = reinterpret_cast<Vec3f*>(vtx + i * vtxSize);
            *meshPos = entities[i]->pos;
        }
    }

}
