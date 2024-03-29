#include <limits>
#include "collision.h"

namespace ramiel {

    void collide(SphereCollider& o1, SphereCollider& o2) {
        //if (&o1 == &o2) return; // check for self collision, not needed

		// collision detection
		Vec3f pos_r = o2.pos - o1.pos;
		float dist = dot(pos_r, pos_r); // dist^2
		float r = o1.hbxrad + o2.hbxrad;
		if (dist > r * r) return;

		// collision depth and time
		float depth = r - std::sqrt(dist);
		float time = depth / mag(o2.posVel - o1.posVel);

		// collision response
		if (o1.responsive) {

			// both responsive
			if (o2.responsive) {
				o1.pos -= o1.posVel * time;
				o2.pos -= o2.posVel * time;
				Vec3f n = normalize(o2.pos - o1.pos);

				const float m = 2.0f / (o1.mass + o2.mass);
				Vec3f v1_p = o1.posVel - n * o2.mass * m * dot(o1.posVel - o2.posVel, n);
				Vec3f v2_p = o2.posVel - n * o1.mass * m * dot(o2.posVel - o1.posVel, n);
				o1.posVel = v1_p;
				o2.posVel = v2_p;

				o1.pos += o1.posVel * time;
				o2.pos += o2.posVel * time;
			}

			// o1 responsive
			else {
				o1.pos -= o1.posVel * time;
				Vec3f n = normalize(o2.pos - o1.pos);
				o1.posVel -= n * 2.0f * dot(o1.posVel, n);
				o1.pos += o1.posVel * time;
			}
			
		}

		// o2 responsive
		else if (o2.responsive) {
			o2.pos -= o2.posVel * time;
			Vec3f n = normalize(o2.pos - o1.pos);
			o2.posVel -= n * 2.0f * dot(o2.posVel, n);
			o2.pos += o2.posVel * time;
		}
    }


    void collide(SphereCollider& sph, AabbCollider& box) {

	}

	
    void collide(SphereCollider& sph, ObbCollider& box) {

	}

	
    void collide(SphereCollider& sph, MeshCollider& mesh) {

	}

	
    void collide(AabbCollider& o1, AabbCollider& o2) {
		Vec3f o1_min = o1.pos - o1.size;
		Vec3f o1_max = o1.pos + o1.size;
		Vec3f o2_min = o2.pos - o2.size;
		Vec3f o2_max = o2.pos + o2.size;

		// collision detection
		if (o1_min[X] > o2_max[X] || o2_min[X] > o1_max[X]) return;
		if (o1_min[Y] > o2_max[Y] || o2_min[Y] > o1_max[Y]) return;
		if (o1_min[Z] > o2_max[Z] || o2_min[Z] > o1_max[Z]) return;

		// collision time, axis of collision
		float  time = std::numeric_limits<float>::max();
		size_t axis = 0;
		for (size_t i = 0; i < 3; ++i) {
			float vel_r = o2.posVel[i] - o1.posVel[i];
			if (!vel_r) continue;
			float depth = vel_r < 0.0f ? o1_max[i] - o2_min[i] : o2_max[i] - o1_min[i];
			float t = std::abs(depth / vel_r);
			if (t < time) {
				time = t;
				axis = i;
			}
		}

		// collision response
		if (o1.responsive) {

			// both responsive
			if (o2.responsive) {
				o1.pos[axis] -= o1.posVel[axis] * time;
				o2.pos[axis] -= o2.posVel[axis] * time;
				
				float m = 1.0f / (o1.mass + o2.mass);
				float v1_p = (o1.mass - o2.mass) * m * o1.posVel[axis] + 2.0f * o2.mass * m * o2.posVel[axis];
				float v2_p = (o2.mass - o1.mass) * m * o2.posVel[axis] + 2.0f * o1.mass * m * o1.posVel[axis];
				o1.posVel[axis] = v1_p;
				o2.posVel[axis] = v2_p;

				o1.pos[axis] += o1.posVel[axis] * time;
				o2.pos[axis] += o2.posVel[axis] * time;
			}

			// o1 responsive
			else {
				o1.pos[axis] -= 2.0f * o1.posVel[axis] * time;
				o1.posVel[axis] *= -1.0f;
			}
			
		}

		// o2 responsive
		else if (o2.responsive) {
			o2.pos[axis] -= 2.0f * o2.posVel[axis] * time;
			o2.posVel[axis] *= -1.0f;
		}
	}

	
    void collide(AabbCollider& aa, ObbCollider& ori) {

	}

	
    void collide(AabbCollider& box, MeshCollider& mesh) {

	}

	
    void collide(ObbCollider& o1, ObbCollider& o2) {

	}

	
    void collide(ObbCollider& box, MeshCollider& mesh) {

	}

	
    void collide(MeshCollider& o1, MeshCollider& o2) {

	}

}
