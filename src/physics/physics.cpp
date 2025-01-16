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

        std::vector<Vec3f> pos(entities.size());

        for (uint32_t step = 0; step < steps; step++) {
            for (size_t i = 0; i < entities.size(); i++) {
                entities[i]->vel[Y] -= 9.8f * dt;
                pos[i] = entities[i]->pos;
                entities[i]->pos += entities[i]->vel * dt;
            }

            for (size_t i = 0; i < constraints.size(); i++) {
                constraints[i]->solve(dt);
            }

            for (size_t i = 0; i < entities.size(); i++) {
                entities[i]->vel = (entities[i]->pos - pos[i]) / dt;
            }
        }
    }

}
