#include "collision.h"

namespace ramiel {

    void collideSphereSphere(SphereCollider& o1, SphereCollider& o2) {
        if (&o1 == &o2) return;

		Vec3f n = o1.pos - o2.pos;
		float n_len = dotProduct(n, n);
		float r = o1.hbxrad + o2.hbxrad;
		if (n_len > r * r) return;
		
		n_len = std::sqrt(n_len);
		//float depth = r - n_len;
		n *= 1.0f / n_len; // normalize

		if (o1.dynamic) {

			// both dynamic
			if (o2.dynamic) {
				const float m = 2.0f / (o1.mass + o2.mass);
				Vec3f v1_p = o1.posVel - n * o2.mass * m * dotProduct(o1.posVel - o2.posVel, n);
				n *= -1.0f;
				Vec3f v2_p = o2.posVel - n * o1.mass * m * dotProduct(o2.posVel - o1.posVel, n);
				o1.posVel = v1_p;
				o2.posVel = v2_p;
			}

			// this dynamic
			else {
				o1.posVel -= n * 2.0f * dotProduct(o1.posVel, n);
			}
				
		}

		// other dynamic
		else if (o2.dynamic) {
			n *= -1.0f;
			o2.posVel -= n * 2.0f * dotProduct(o2.posVel, n);
		}
    }

}
