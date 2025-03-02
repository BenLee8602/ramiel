#pragma once

#include <ramiel/math.h>

namespace ramiel {

    class Particle {
    public:
        Vec3f pos;
        Vec3f vel;
        float wass;

        Vec3f posPrev;

        Particle(
            const Vec3f& pos = Vec3f{},
            const Vec3f& vel = Vec3f{},
            float wass = 1.0f
        );

        virtual void integrate(float dt);
        virtual void update(float dt);
        virtual Mat4x4f getTransform() const;
    };


    class RigidBody : public Particle {
    public:
        Quaternion rot;
        Vec3f rotAxis;
        Vec3f woi;

        Quaternion rotPrev;

        RigidBody(
            const Vec3f& pos = Vec3f{},
            const Vec3f& vel = Vec3f{},
            const Quaternion& rot = qtn(),
            const Vec3f& rotAxis = Vec3f{},
            float wass = 1.0f,
            const Vec3f& woi = { 1, 1, 1 }
        );

        virtual void integrate(float dt) override;
        virtual void update(float dt) override;
        virtual Mat4x4f getTransform() const override;
    };

}
