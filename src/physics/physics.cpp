#include <cassert>
#include <vector>
#include <algorithm>

#include "physics.h"
#include "constraint.h"
#include "entity.h"
using namespace ramiel;

namespace {

    std::vector<Constraint*> constraints;
    std::vector<Particle*> entities;


    template<class T>
    bool hasValue(const std::vector<T>& c, const T& v) {
        return std::find(c.begin(), c.end(), v) == c.end();
    }

    template<class T>
    void removeByValue(std::vector<T>& c, const T& v) {
        c.erase(std::remove(c.begin(), c.end(), v), c.end());
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
        assert(!hasValue(entities, entity));
        entities.push_back(entity);
    }

    void removeEntity(Particle* entity) {
        assert(hasValue(entities, entity));
        removeByValue(entities, entity);
    }


    void simulatePhysics(float dt, uint32_t steps) {
        dt /= steps;

        for (uint32_t step = 0; step < steps; step++) {
            for (auto& e : entities) {
                e->integrate(dt);
            }

            for (auto& c : constraints) {
                c->solve(dt);
            }

            for (auto& e : entities) {
                e->update(dt);
            }
        }
    }

}
