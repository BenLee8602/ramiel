#include <string>
#include <algorithm>
#include "physicsobj.h"
#include "collision.h"
#include "physics.h"
#include "graphics.h"

namespace ramiel {

    PhysicsObj::PhysicsObj(
        bool dynamic,
        Vec3f pos, Vec3f rot,
        Vec3f posVel, Vec3f rotVel,
        Vec3f posAcc, Vec3f rotAcc,
        float mass
    ) {
        this->dynamic = dynamic;
        this->pos = pos;
        this->rot = rot;
        this->posVel = posVel;
        this->rotVel = rotVel;
        this->posAcc = posAcc;
        this->rotAcc = rotAcc;
        this->mass = mass;

        physics::physicsObjs.push_back(this);
        if (dynamic) physics::dynamicObjs.push_back(this);
    }


    PhysicsObj::~PhysicsObj() {
        using namespace physics;
        
        auto i = std::find(physicsObjs.begin(), physicsObjs.end(), this);
        if (i != physicsObjs.end()) physicsObjs.erase(i);

        if (dynamic) {
            i = std::find(dynamicObjs.begin(), dynamicObjs.end(), this);
            if (i != dynamicObjs.end()) dynamicObjs.erase(i);
        }
    }


    void PhysicsObj::step() {
        posVel += posAcc * physics::dtime;
        rotVel += rotAcc * physics::dtime;
        pos    += posVel * physics::dtime;
        rot    += rotVel * physics::dtime;
    }


    void PhysicsObj::collideWith(PhysicsObj* other) {
        throw std::string("PhysicsObj::collideWith called without collider");
    }


    void PhysicsObj::collideWith(SphereCollider* other) {
        throw std::string("PhysicsObj::collideWith called without collider");
    }


    SphereCollider::SphereCollider(
        bool dynamic,
        Vec3f pos, Vec3f rot,
        Vec3f posVel, Vec3f rotVel,
        Vec3f posAcc, Vec3f rotAcc,
        float mass, float hbxrad
    ) : PhysicsObj(
        dynamic,
        pos, rot,
        posVel, rotVel,
        posAcc, rotAcc,
        mass
    ) {
        this->hbxrad = hbxrad;
        physics::collidableObjs.push_back(this);
    }


    SphereCollider::~SphereCollider() {
        using namespace physics;
        auto i = std::find(collidableObjs.begin(), collidableObjs.end(), this);
        if (i != collidableObjs.end()) collidableObjs.erase(i);
    }


    void SphereCollider::collideWith(PhysicsObj* other) {
        other->collideWith(this);
    }


    void SphereCollider::collideWith(SphereCollider* other) {
        collideSphereSphere(*this, *other);
    }

}
