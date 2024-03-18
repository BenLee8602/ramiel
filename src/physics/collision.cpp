#include <limits>
#include "collision.h"

namespace ramiel {

    Collide_Sph_Sph::Collide_Sph_Sph(Collider* sphere1, Collider* sphere2)
        : sphere1(*static_cast<SphereCollider*>(sphere1))
        , sphere2(*static_cast<SphereCollider*>(sphere2))
        , time(0.0f)
    {}


    bool Collide_Sph_Sph::detect() {
        Vec3f pos_r = sphere2.pos - sphere1.pos;
        float dist = dot(pos_r, pos_r); // dist^2
        float r = sphere1.hbxrad + sphere2.hbxrad;
        if (dist > r * r) return false;

        float depth = r - std::sqrt(dist);
        time = depth / mag(sphere2.posVel - sphere1.posVel);

        return true;
    }

    
    void Collide_Sph_Sph::resolve() {
        sphere1.pos -= sphere1.posVel * time;
        sphere2.pos -= sphere2.posVel * time;
        Vec3f n = normalize(sphere2.pos - sphere1.pos);

        const float m = 2.0f / (sphere1.mass + sphere2.mass);
        Vec3f v1_p = sphere1.posVel - n * sphere2.mass * m * dot(sphere1.posVel - sphere2.posVel, n);
        Vec3f v2_p = sphere2.posVel - n * sphere1.mass * m * dot(sphere2.posVel - sphere1.posVel, n);
        sphere1.posVel = v1_p;
        sphere2.posVel = v2_p;

        sphere1.pos += sphere1.posVel * time;
        sphere2.pos += sphere2.posVel * time;
    }


    Collide_Aabb_Aabb::Collide_Aabb_Aabb(Collider* box1, Collider* box2)
        : box1(*static_cast<AabbCollider*>(box1))
        , box2(*static_cast<AabbCollider*>(box2))
        , time(std::numeric_limits<float>::max())
        , axis(0)
    {}

    
    bool Collide_Aabb_Aabb::detect() {
        Vec3f box1_min = box1.pos - box1.size;
        Vec3f box1_max = box1.pos + box1.size;
        Vec3f box2_min = box2.pos - box2.size;
        Vec3f box2_max = box2.pos + box2.size;

        if (box1_min[X] > box2_max[X] || box2_min[X] > box1_max[X]) return false;
        if (box1_min[Y] > box2_max[Y] || box2_min[Y] > box1_max[Y]) return false;
        if (box1_min[Z] > box2_max[Z] || box2_min[Z] > box1_max[Z]) return false;

        for (size_t i = 0; i < 3; ++i) {
            float vel_r = box2.posVel[i] - box1.posVel[i];
            if (!vel_r) continue;
            float depth = vel_r < 0.0f ? box1_max[i] - box2_min[i] : box2_max[i] - box1_min[i];
            float t = std::abs(depth / vel_r);
            if (t < time) {
                time = t;
                axis = i;
            }
        }

        return true;
    }

    
    void Collide_Aabb_Aabb::resolve() {
        box1.pos[axis] -= box1.posVel[axis] * time;
        box2.pos[axis] -= box2.posVel[axis] * time;
        
        float m = 1.0f / (box1.mass + box2.mass);
        float v1_p = (box1.mass - box2.mass) * m * box1.posVel[axis] + 2.0f * box2.mass * m * box2.posVel[axis];
        float v2_p = (box2.mass - box1.mass) * m * box2.posVel[axis] + 2.0f * box1.mass * m * box1.posVel[axis];
        box1.posVel[axis] = v1_p;
        box2.posVel[axis] = v2_p;

        box1.pos[axis] += box1.posVel[axis] * time;
        box2.pos[axis] += box2.posVel[axis] * time;
    }

}
