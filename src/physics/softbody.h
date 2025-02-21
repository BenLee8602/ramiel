#pragma once

#include <memory>
#include <vector>
#include <ramiel/data.h>

namespace ramiel {

    class Constraint;
    class Particle;
    class ParticleCollider;

    class SoftBody {
    public:
        SoftBody(const Mesh& mesh, float k);
        ~SoftBody();

        SoftBody(const SoftBody&) = delete;
        SoftBody& operator=(const SoftBody&) = delete;

        SoftBody(SoftBody&&) = default;
        SoftBody& operator=(SoftBody&&) = default;

        operator bool() const;
        void enable();
        void disable();

        std::shared_ptr<Mesh> getMesh();
        void updateMesh();

    public:
        std::shared_ptr<Mesh> mesh;
        bool enabled;

        std::vector<Constraint*> constraints;
        std::vector<Particle*> entities;
        std::vector<ParticleCollider*> colliders;
    };

}
