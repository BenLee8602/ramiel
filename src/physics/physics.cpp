#include <cassert>
#include <vector>
#include <algorithm>

#include "physics.h"
#include "constraint.h"
#include "entity.h"
using namespace ramiel;

namespace {

    using ColliderTypePair = std::pair<Collider::Type, Collider::Type>;

    class HashColliderTypePair {
    public:
        size_t operator()(const ColliderTypePair& key) const {
            size_t hash1 = key.first.hash_code();
            size_t hash2 = key.second.hash_code();
            return hash1 ^ (hash2 << 2);
        }
    };

    using CollisionDetectorMap = std::unordered_map<
        ColliderTypePair,
        CollisionDetector,
        HashColliderTypePair
    >;


    std::vector<Constraint*> constraints;
    
    std::vector<Particle*> particles;
    std::vector<RigidBody*> rigidBodies;
    
    std::vector<Collider*> colliders;
    CollisionDetectorMap collisionDetectors;
    BroadCollisionFn broadCollisionFn;


    template<class T>
    bool hasValue(const std::vector<T>& c, const T& v) {
        return std::find(c.begin(), c.end(), v) != c.end();
    }

    template<class T>
    void removeByValue(std::vector<T>& c, const T& v) {
        c.erase(std::remove(c.begin(), c.end(), v), c.end());
    }


    CollisionDetector flipCollisionDetector(CollisionDetector detector) {
        return [detector](Collider* c1, Collider* c2) {
            return detector(c2, c1);
        };
    }

    CollisionDetector findCollisionDetector(
        Collider::Type t1,
        Collider::Type t2
    ) {
        auto detector = collisionDetectors.find({ t1, t2 });
        if (detector != collisionDetectors.end()) return detector->second;
        
        detector = collisionDetectors.find({ t2, t1 });
        if (detector != collisionDetectors.end())
            return flipCollisionDetector(detector->second);

        return nullptr;
    }

    void insertCollisionDetector(
        Collider::Type t1,
        Collider::Type t2,
        CollisionDetector detector
    ) {
        assert(!findCollisionDetector(t1, t2));
        collisionDetectors.insert({ { t1, t2 }, detector });
    }

    void eraseCollisionDetector(
        Collider::Type t1,
        Collider::Type t2
    ) {
        assert(findCollisionDetector(t1, t2));
        collisionDetectors.erase({ t1, t2 });
        collisionDetectors.erase({ t2, t1 });
    }

}

namespace ramiel {

    void addConstraint(Constraint* constraint) {
        assert(constraint);
        assert(!hasValue(constraints, constraint));
        constraints.push_back(constraint);
    }

    void removeConstraint(Constraint* constraint) {
        assert(hasValue(constraints, constraint));
        removeByValue(constraints, constraint);
    }


    void addEntity(Particle* entity) {
        assert(entity);
        assert(!hasValue(particles, entity));
        particles.push_back(entity);
    }

    void removeEntity(Particle* entity) {
        assert(hasValue(particles, entity));
        removeByValue(particles, entity);
    }


    void addEntity(RigidBody* entity) {
        assert(entity);
        assert(!hasValue(rigidBodies, entity));
        rigidBodies.push_back(entity);
    }

    void removeEntity(RigidBody* entity) {
        assert(hasValue(rigidBodies, entity));
        removeByValue(rigidBodies, entity);
    }


    void addCollider(Collider* collider) {
        assert(collider);
        assert(!hasValue(colliders, collider));
        colliders.push_back(collider);
    }

    void removeCollider(Collider* collider) {
        assert(hasValue(colliders, collider));
        removeByValue(colliders, collider);
    }


    void addCollisionDetector(
        Collider::Type t1,
        Collider::Type t2,
        CollisionDetector detector
    ) {
        insertCollisionDetector(t1, t2, detector);
    }

    void removeCollisionDetector(
        Collider::Type t1,
        Collider::Type t2
    ) {
        eraseCollisionDetector(t1, t2);
    }


    void setBroadCollisionFn(BroadCollisionFn fn) {
        assert(fn);
        broadCollisionFn = fn;
    }


    void simulatePhysics(float dt) {
        for (auto e : particles) e->integrate(dt);
        for (auto e : rigidBodies) e->integrate(dt);

        assert(broadCollisionFn);
        auto colliderPairs = broadCollisionFn(colliders);
        std::vector<Constraint*> collisionConstraints;
        collisionConstraints.reserve(colliderPairs.size());

        for (auto& cp : colliderPairs) {
            CollisionDetector detector = findCollisionDetector(
                cp.first->getColliderType(),
                cp.second->getColliderType()
            );
            if (!detector) continue;

            Constraint* constraint = detector(cp.first, cp.second);
            if (!constraint) continue;

            collisionConstraints.emplace_back(std::move(constraint));
            constraints.push_back(collisionConstraints[collisionConstraints.size() - 1]);
        }

        for (auto c : constraints) c->solve(dt);

        for (auto& c : collisionConstraints) delete c;
        constraints.erase(
            constraints.end() - collisionConstraints.size(),
            constraints.end()
        );

        for (auto e : particles) e->update(dt);
        for (auto e : rigidBodies) e->update(dt);
    }

    void simulatePhysics(float dt, uint32_t steps) {
        dt /= steps;
        for (uint32_t step = 0; step < steps; step++) {
            simulatePhysics(dt);
        }
    }

}
