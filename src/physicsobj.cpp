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
        Vec3f posAcc, Vec3f rotAcc
    ) {
        this->dynamic = dynamic;
        this->pos = pos;
        this->rot = rot;
        this->posVel = posVel;
        this->rotVel = rotVel;
        this->posAcc = posAcc;
        this->rotAcc = rotAcc;

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


    Collider::Collider(
        bool dynamic,
        Vec3f pos, Vec3f rot,
        Vec3f posVel, Vec3f rotVel,
        Vec3f posAcc, Vec3f rotAcc,
        float mass
    ) : PhysicsObj(
        dynamic,
        pos, rot,
        posVel, rotVel,
        posAcc, rotAcc
    ) {
        this->mass = mass;
        physics::collidableObjs.push_back(this);
    }
    

    Collider::~Collider() {
        using namespace physics;
        auto i = std::find(collidableObjs.begin(), collidableObjs.end(), this);
        if (i != collidableObjs.end()) collidableObjs.erase(i);
    }


    void Collider::collideWith(Collider* other) {
        throw std::string("collideWith called without collider");
    }


    void Collider::collideWith(SphereCollider* other) {
        throw std::string("collideWith called without collider");
    }


    void SphereCollider::collideWith(Collider* other) {
        other->collideWith(this);
    }


    void SphereCollider::collideWith(SphereCollider* other) {
        collideSphereSphere(*this, *other);
    }

}
