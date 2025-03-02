#include "entity.h"

namespace ramiel {

    Particle::Particle(
        const Vec3f& pos,
        const Vec3f& vel,
        float wass
    )
        : pos(pos)
        , vel(vel)
        , wass(wass)
        , posPrev(Vec3f{})
    {}


    void Particle::integrate(float dt) {
        posPrev = pos;
        //vel[Y] -= 9.8f * dt;
        pos += vel * dt;
    }

    void Particle::update(float dt) {
        vel = (pos - posPrev) / dt;
    }

    Mat4x4f Particle::getTransform() const {
        return translate(pos);
    }


    RigidBody::RigidBody(
        const Vec3f& pos,
        const Vec3f& vel,
        const Quaternion& rot,
        const Vec3f& rotAxis,
        float wass,
        const Vec3f& woi
    )
        : Particle(pos, vel, wass)
        , rot(rot)
        , rotAxis(rotAxis)
        , rotPrev(qtn())
        , woi(woi)
    {}


    void RigidBody::integrate(float dt) {
        Particle::integrate(dt);
        rotPrev = rot;
        Quaternion drot = { rotAxis[X], rotAxis[Y], rotAxis[Z], 0.0f };
        rot = normalize(rot + qtnqtn(drot, rot) * 0.5f * dt);
    }

    void RigidBody::update(float dt) {
        Particle::update(dt);
        Quaternion drot = qtnqtn(rot, qtninv(rotPrev));
        rotAxis = sizeView<3>(drot) * 2.0f / dt;
        if (drot[W] < 0.0f) rotAxis = -rotAxis;
    }

    Mat4x4f RigidBody::getTransform() const {
        return matmat(qtnmat(rot), Particle::getTransform());
    }

}
