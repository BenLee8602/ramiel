#include <limits>
#include "collision.h"

namespace ramiel {

    void collideSphereSphere(Collider* collider1, Collider* collider2) {
		SphereCollider& sphere1 = *static_cast<SphereCollider*>(collider1);
		SphereCollider& sphere2 = *static_cast<SphereCollider*>(collider2);

		// collision detection
		Vec3f pos_r = sphere2.pos - sphere1.pos;
		float dist = dot(pos_r, pos_r); // dist^2
		float r = sphere1.hbxrad + sphere2.hbxrad;
		if (dist > r * r) return;

		// collision depth and time
		float depth = r - std::sqrt(dist);
		float time = depth / mag(sphere2.posVel - sphere1.posVel);

		// collision response
		if (sphere1.responsive) {

			// both responsive
			if (sphere2.responsive) {
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

			// sphere1 responsive
			else {
				sphere1.pos -= sphere1.posVel * time;
				Vec3f n = normalize(sphere2.pos - sphere1.pos);
				sphere1.posVel -= n * 2.0f * dot(sphere1.posVel, n);
				sphere1.pos += sphere1.posVel * time;
			}
			
		}

		// sphere2 responsive
		else if (sphere2.responsive) {
			sphere2.pos -= sphere2.posVel * time;
			Vec3f n = normalize(sphere2.pos - sphere1.pos);
			sphere2.posVel -= n * 2.0f * dot(sphere2.posVel, n);
			sphere2.pos += sphere2.posVel * time;
		}
	}


    void collideAabbAabb(Collider* collider1, Collider* collider2) {
		AabbCollider& aabb1 = *static_cast<AabbCollider*>(collider1);
		AabbCollider& aabb2 = *static_cast<AabbCollider*>(collider2);
		
		Vec3f aabb1_min = aabb1.pos - aabb1.size;
		Vec3f aabb1_max = aabb1.pos + aabb1.size;
		Vec3f aabb2_min = aabb2.pos - aabb2.size;
		Vec3f aabb2_max = aabb2.pos + aabb2.size;

		// collision detection
		if (aabb1_min[X] > aabb2_max[X] || aabb2_min[X] > aabb1_max[X]) return;
		if (aabb1_min[Y] > aabb2_max[Y] || aabb2_min[Y] > aabb1_max[Y]) return;
		if (aabb1_min[Z] > aabb2_max[Z] || aabb2_min[Z] > aabb1_max[Z]) return;

		// collision time, axis of collision
		float  time = std::numeric_limits<float>::max();
		size_t axis = 0;
		for (size_t i = 0; i < 3; ++i) {
			float vel_r = aabb2.posVel[i] - aabb1.posVel[i];
			if (!vel_r) continue;
			float depth = vel_r < 0.0f ? aabb1_max[i] - aabb2_min[i] : aabb2_max[i] - aabb1_min[i];
			float t = std::abs(depth / vel_r);
			if (t < time) {
				time = t;
				axis = i;
			}
		}

		// collision response
		if (aabb1.responsive) {

			// both responsive
			if (aabb2.responsive) {
				aabb1.pos[axis] -= aabb1.posVel[axis] * time;
				aabb2.pos[axis] -= aabb2.posVel[axis] * time;
				
				float m = 1.0f / (aabb1.mass + aabb2.mass);
				float v1_p = (aabb1.mass - aabb2.mass) * m * aabb1.posVel[axis] + 2.0f * aabb2.mass * m * aabb2.posVel[axis];
				float v2_p = (aabb2.mass - aabb1.mass) * m * aabb2.posVel[axis] + 2.0f * aabb1.mass * m * aabb1.posVel[axis];
				aabb1.posVel[axis] = v1_p;
				aabb2.posVel[axis] = v2_p;

				aabb1.pos[axis] += aabb1.posVel[axis] * time;
				aabb2.pos[axis] += aabb2.posVel[axis] * time;
			}

			// aabb1 responsive
			else {
				aabb1.pos[axis] -= 2.0f * aabb1.posVel[axis] * time;
				aabb1.posVel[axis] *= -1.0f;
			}
			
		}

		// aabb2 responsive
		else if (aabb2.responsive) {
			aabb2.pos[axis] -= 2.0f * aabb2.posVel[axis] * time;
			aabb2.posVel[axis] *= -1.0f;
		}
	}

}
