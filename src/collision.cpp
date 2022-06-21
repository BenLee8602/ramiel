#include "collision.h"

namespace ramiel {

    void collideSphereSphere(SphereCollider& o1, SphereCollider& o2) {
        //if (&o1 == &o2) return; // check for self collision, not needed

		// collision detection
		Vec3f pos_r = o2.pos - o1.pos;
		float dist = dotProduct(pos_r, pos_r); // dist^2
		float r = o1.hbxrad + o2.hbxrad;
		if (dist > r * r) return;

		// calculate time of collision
		float depth = r - std::sqrt(dist);
		float time = depth / getMagnitude(o2.posVel - o1.posVel);

		// collision response
		if (o1.dynamic) {

			// both dynamic
			if (o2.dynamic) {
				o1.pos -= o1.posVel * time;
				o2.pos -= o2.posVel * time;
				Vec3f n = getNormalized(o2.pos - o1.pos);

				const float m = 2.0f / (o1.mass + o2.mass);
				Vec3f v1_p = o1.posVel - n * o2.mass * m * dotProduct(o1.posVel - o2.posVel, n);
				Vec3f v2_p = o2.posVel - n * o1.mass * m * dotProduct(o2.posVel - o1.posVel, n);
				o1.posVel = v1_p;
				o2.posVel = v2_p;

				o1.pos += o1.posVel * time;
				o2.pos += o2.posVel * time;
			}

			// o1 dynamic
			else {
				o1.pos -= o1.posVel * time;
				Vec3f n = getNormalized(o2.pos - o1.pos);
				o1.posVel -= n * 2.0f * dotProduct(o1.posVel, n);
				o1.pos += o1.posVel * time;
			}
			
		}

		// o2 dynamic
		else if (o2.dynamic) {
			o2.pos -= o2.posVel * time;
			Vec3f n = getNormalized(o2.pos - o1.pos);
			o2.posVel -= n * 2.0f * dotProduct(o2.posVel, n);
			o2.pos += o2.posVel * time;
		}
    }


    void collideSphereAabb(SphereCollider& sph, AabbCollider& box) {

	}

	
    void collideSphereObb(SphereCollider& sph, ObbCollider& box) {

	}

	
    void collideSphereMesh(SphereCollider& sph, MeshCollider& mesh) {

	}

	
    void collideAabbAabb(AabbCollider& o1, AabbCollider& o2) {
		// collision detection
		if (o1.min[X] > o2.max[X] || o2.min[X] > o1.max[X]) return;
		if (o1.min[Y] > o2.max[Y] || o2.min[Y] > o1.max[Y]) return;
		if (o1.min[Z] > o2.max[Z] || o2.min[Z] > o1.max[Z]) return;
	}

	
    void collideAabbObb(AabbCollider& aa, ObbCollider& ori) {

	}

	
    void collideAabbMesh(AabbCollider& box, MeshCollider& mesh) {

	}

	
    void collideObbObb(ObbCollider& o1, ObbCollider& o2) {

	}

	
    void collideObbMesh(ObbCollider& box, MeshCollider& mesh) {

	}

	
    void collideMeshMesh(MeshCollider& o1, MeshCollider& o2) {

	}

}
