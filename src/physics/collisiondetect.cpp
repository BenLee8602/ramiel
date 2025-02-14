#include <array>
#include <cassert>

#include "collisiondetect.h"
#include "collisionresolve.h"
#include "constraint.h"
#include "collider.h"
using namespace ramiel;

namespace {

    template<class ColliderType>
    bool cldrIs(const Collider* c) {
        return c->getColliderType() == typeid(ColliderType);
    }


    Constraint* collideParticlePlaneImpl(Collider* particle_, Collider* plane_) {
        assert(particle_ && cldrIs<ParticleCollider>(particle_));
        assert(plane_ && cldrIs<PlaneCollider>(plane_));
        auto particle = static_cast<ParticleCollider*>(particle_);
        auto plane = static_cast<PlaneCollider*>(plane_);
        assert(particle->e);

        Vec3f& v = particle->e->pos;
        Vec3f dn = plane->n * plane->d;
        
        if (dot(plane->n, dn - v) <= 0.0f) return nullptr;

        Vec3f p = v - plane->n * (dot(plane->n, v) - plane->d);
        return new CollisionConstraint_P(p, particle->e);
    }


    Constraint* collideParticleSphereImpl(Collider* particle_, Collider* sphere_) {
        assert(particle_ && cldrIs<ParticleCollider>(particle_));
        assert(sphere_ && cldrIs<SphereCollider>(sphere_));
        auto particle = static_cast<ParticleCollider*>(particle_);
        auto sphere = static_cast<SphereCollider*>(sphere_);
        assert(particle->e);
        assert(sphere->e);

        Vec3f n = particle->e->pos - sphere->e->pos;
        float d = dot(n, n);
        float r = sphere->r;

        if (d >= r * r) return nullptr;

        d = std::sqrt(d);
        n = normalize(n, d);

        Vec3f c = sphere->e->pos + n * r;
        return new CollisionConstraint_PR(particle->e, sphere->e, c);
    }


    Constraint* collideParticleBoxImpl(Collider* particle_, Collider* box_) {
        assert(particle_ && cldrIs<ParticleCollider>(particle_));
        assert(box_ && cldrIs<BoxCollider>(box_));
        auto particle = static_cast<ParticleCollider*>(particle_);
        auto box = static_cast<BoxCollider*>(box_);
        assert(particle->e);
        assert(box->e);
        
        Vec3f pos = particle->e->pos - box->e->pos;
        pos = qtnvec(qtninv(box->e->rot), pos);

        Vec3f c;
        float dmin = std::numeric_limits<float>::max();
        for (size_t i = 0; i < 3; i++) {
            float d = box->size[i] + pos[i];
            if (d <= 0.0f) return nullptr;
            if (d < dmin) {
                dmin = d;
                c = pos;
                c[i] = -box->size[i];
            }

            d = box->size[i] - pos[i];
            if (d <= 0.0f) return nullptr;
            if (d < dmin) {
                dmin = d;
                c = pos;
                c[i] = box->size[i];
            }
        }

        c = qtnvec(box->e->rot, c) + box->e->pos;
        return new CollisionConstraint_PR(particle->e, box->e, c);
    }


    Constraint* collidePlaneSphereImpl(Collider* plane_, Collider* sphere_) {
        assert(plane_ && cldrIs<PlaneCollider>(plane_));
        assert(sphere_ && cldrIs<SphereCollider>(sphere_));
        auto plane = static_cast<PlaneCollider*>(plane_);
        auto sphere = static_cast<SphereCollider*>(sphere_);
        assert(sphere->e);

        Vec3f v = sphere->e->pos - plane->n * sphere->r;
        Vec3f dn = plane->n * plane->d;
        
        if (dot(plane->n, dn - v) <= 0.0f) return nullptr;

        Vec3f p = v - plane->n * (dot(plane->n, v) - plane->d);
        return new CollisionConstraint_R(p, sphere->e, v);
    }


    Constraint* collidePlaneBoxImpl(Collider* plane_, Collider* box_) {
        assert(plane_ && cldrIs<PlaneCollider>(plane_));
        assert(box_ && cldrIs<BoxCollider>(box_));
        auto plane = static_cast<PlaneCollider*>(plane_);
        auto box = static_cast<BoxCollider*>(box_);
        assert(box->e);

        Vec3f& bs = box->size;
        std::array<Vec3f, 8> vtx = {
            Vec3f{ -bs[X], -bs[Y], -bs[Z] },
            Vec3f{ -bs[X], -bs[Y],  bs[Z] },
            Vec3f{ -bs[X],  bs[Y], -bs[Z] },
            Vec3f{ -bs[X],  bs[Y],  bs[Z] },
            Vec3f{  bs[X], -bs[Y], -bs[Z] },
            Vec3f{  bs[X], -bs[Y],  bs[Z] },
            Vec3f{  bs[X],  bs[Y], -bs[Z] },
            Vec3f{  bs[X],  bs[Y],  bs[Z] }
        };

        Vec3f dn = plane->n * plane->d;

        float dmax = 0.0f;
        Vec3f c0;
        Vec3f c1;
        for (auto& v : vtx) {
            v = qtnvec(box->e->rot, v) + box->e->pos;
            if (dot(plane->n, dn - v) <= 0.0f) continue;

            float d = dot(plane->n, v) - plane->d;
            if (std::abs(d) <= dmax) continue;

            dmax = std::abs(d);
            c0 = v - plane->n * d;
            c1 = v;
        }

        if (!dmax) return nullptr;
        return new CollisionConstraint_R(c0, box->e, c1);
    }


    Constraint* collideSphereSphereImpl(Collider* sphere0_, Collider* sphere1_) {
        assert(sphere0_ && cldrIs<SphereCollider>(sphere0_));
        assert(sphere1_ && cldrIs<SphereCollider>(sphere1_));
        auto sphere0 = static_cast<SphereCollider*>(sphere0_);
        auto sphere1 = static_cast<SphereCollider*>(sphere1_);
        assert(sphere0->e);
        assert(sphere1->e);

        Vec3f n = sphere1->e->pos - sphere0->e->pos;
        float d = dot(n, n);
        float r = sphere0->r + sphere1->r;

        if (d >= r * r) return nullptr;

        d = std::sqrt(d);
        n = normalize(n, d);

        Vec3f c0 = sphere0->e->pos + n * sphere0->r;
        Vec3f c1 = sphere1->e->pos - n * sphere1->r;
        return new CollisionConstraint_RR(sphere0->e, sphere1->e, c0, c1);
    }


    Constraint* collideSphereBoxImpl(Collider* sphere_, Collider* box_) {
        assert(sphere_ && cldrIs<SphereCollider>(sphere_));
        assert(box_ && cldrIs<BoxCollider>(box_));
        auto sphere = static_cast<SphereCollider*>(sphere_);
        auto box = static_cast<BoxCollider*>(box_);
        assert(sphere->e);
        assert(box->e);

        // this is kinda complicated so im leaving this stub for now
        return nullptr;
    }


    Constraint* collideBoxBoxImpl(Collider* box0_, Collider* box1_) {
        assert(box0_ && cldrIs<BoxCollider>(box0_));
        assert(box1_ && cldrIs<BoxCollider>(box1_));
        auto box0 = static_cast<BoxCollider*>(box0_);
        auto box1 = static_cast<BoxCollider*>(box1_);
        assert(box0->e);
        assert(box1->e);

        // this is kinda complicated so im leaving this stub for now
        return nullptr;
    }

}

namespace ramiel {

    CollisionDetector collideParticlePlane() {
        return collideParticlePlaneImpl;
    }

    CollisionDetector collideParticleSphere() {
        return collideParticleSphereImpl;
    }

    CollisionDetector collideParticleBox() {
        return collideParticleBoxImpl;
    }

    CollisionDetector collidePlaneSphere() {
        return collidePlaneSphereImpl;
    }

    CollisionDetector collidePlaneBox() {
        return collidePlaneBoxImpl;
    }

    CollisionDetector collideSphereSphere() {
        return collideSphereSphereImpl;
    }

    CollisionDetector collideSphereBox() {
        return collideSphereBoxImpl;
    }

    CollisionDetector collideBoxBox() {
        return collideBoxBoxImpl;
    }

}
