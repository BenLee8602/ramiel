#pragma once

#include <typeindex>
#include <ramiel/math.h>

namespace ramiel {

    struct PhysicsObject {
        Vec3f pos;
        Vec3f rot;

        PhysicsObject(
            Vec3f pos = Vec3f(),
            Vec3f rot = Vec3f()
        ) :
            pos(pos),
            rot(rot)
        {}

        inline Mat4x4f getTransform() const {
            return matmat(translate(pos), rotate(rot));
        }

        virtual ~PhysicsObject() {}
    };


    struct Kinematics : public PhysicsObject {
        Vec3f pv;
        Vec3f rv;
        Vec3f pa;
        Vec3f ra;

        Kinematics(
            Vec3f pos = Vec3f(),
            Vec3f rot = Vec3f(),
            Vec3f pv  = Vec3f(),
            Vec3f rv  = Vec3f(),
            Vec3f pa  = Vec3f(),
            Vec3f ra  = Vec3f()
        ) :
            PhysicsObject(pos, rot),
            pv(pv),
            rv(rv),
            pa(pa),
            ra(ra)
        {}

        inline void step(float dtime) {
            pv  += pa * dtime;
            rv  += ra * dtime;
            pos += pv * dtime;
            rot += rv * dtime;
        }

        virtual ~Kinematics() {}
    };
    
    
    typedef std::type_index ColliderType;

    struct Collider : public Kinematics {
        Collider(Kinematics kinematics) : Kinematics(kinematics) {}
        virtual ColliderType getType() const = 0;
        virtual ~Collider() {}
    };

}
