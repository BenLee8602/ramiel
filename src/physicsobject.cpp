#include "scene.h"

namespace ramiel {

    PhysicsObject::PhysicsObject(Scene& scene, Vec3f pos, Rotation rot) : scene(scene) {
        this->pos = pos;
        this->rot = rot;
        this->posVel = vec3f_0;
        this->rotVel = vec3f_0;
        this->posAcc = vec3f_0;
        this->rotAcc = vec3f_0;

        scene.addPhysicsObject(this);
    }


    PhysicsObject::~PhysicsObject() {
        scene.removePhysicsObject(this);
    }


    void PhysicsObject::step(float dtime) {
        posVel += posAcc * dtime;
        rotVel += rotAcc * dtime;
        pos    += posVel * dtime;
        rot    += rotVel * dtime;
    }


    Collider::Collider(
        Scene& scene,
        Vec3f pos,
        Rotation rot,
        bool responsive,
        float mass
    ) : PhysicsObject(scene, pos, rot) {
        this->responsive = responsive;
        this->mass = mass;
        scene.addCollider(this);
    }
    

    Collider::~Collider() {
        scene.removeCollider(this);
    }


    void SphereCollider::collideWith(Collider* other)       { other->collideWith(this); }
    void SphereCollider::collideWith(SphereCollider* other) { collideSphereSphere(*this, *other); }
    void SphereCollider::collideWith(AabbCollider* other)   { collideSphereAabb(*this, *other); }
    void SphereCollider::collideWith(ObbCollider* other)    { collideSphereObb(*this, *other); }
    void SphereCollider::collideWith(MeshCollider* other)   { collideSphereMesh(*this, *other); }

    void AabbCollider::collideWith(Collider* other)         { other->collideWith(this); }
    void AabbCollider::collideWith(SphereCollider* other)   { collideSphereAabb(*other, *this); }
    void AabbCollider::collideWith(AabbCollider* other)     { collideAabbAabb(*this, *other); }
    void AabbCollider::collideWith(ObbCollider* other)      { collideAabbObb(*this, *other); }
    void AabbCollider::collideWith(MeshCollider* other)     { collideAabbMesh(*this, *other); }
    
    void ObbCollider::collideWith(Collider* other)          { other->collideWith(this); }
    void ObbCollider::collideWith(SphereCollider* other)    { collideSphereObb(*other, *this); }
    void ObbCollider::collideWith(AabbCollider* other)      { collideAabbObb(*other, *this); }
    void ObbCollider::collideWith(ObbCollider* other)       { collideObbObb(*this, *other); }
    void ObbCollider::collideWith(MeshCollider* other)      { collideObbMesh(*this, *other); }

    void MeshCollider::collideWith(Collider* other)         { other->collideWith(this); }
    void MeshCollider::collideWith(SphereCollider* other)   { collideSphereMesh(*other, *this); }
    void MeshCollider::collideWith(AabbCollider* other)     { collideAabbMesh(*other, *this); }
    void MeshCollider::collideWith(ObbCollider* other)      { collideObbMesh(*other, *this); }
    void MeshCollider::collideWith(MeshCollider* other)     { collideMeshMesh(*this, *other); }

}
