#include "graphicsbl_p.h"

namespace bl {

	void Physics::simulateMovement() {
		if (!movement) return;
		pos += velocity * GraphicsBL::dtime;
		velocity += acceleration * GraphicsBL::dtime;
	}


	void Physics::simulateCollision(Physics& other) {
		if (this == &other) return;

		// collision normal
		Vec3f n = pos - other.pos;
		if (getMagnitude(n) > hbxrad + other.hbxrad) return;
		n = getNormalized(n);

		if (movement) {

			// both moveable
			if (other.movement) {
				Vec3f& v1 = velocity;
				Vec3f& v2 = other.velocity;
				float& m1 = mass;
				float& m2 = other.mass;
				const float m = 2.0f / (mass + other.mass);

				Vec3f v1_p = v1 - n * m2 * m * dotProduct(v1 - v2, n);
				n *= -1.0f;
				Vec3f v2_p = v2 - n * m1 * m * dotProduct(v2 - v1, n);
				v1 = v1_p;
				v2 = v2_p;
			}

			// this moveable
			else {
				velocity -= n * 2.0f * dotProduct(velocity, n);
			}
				
		}

		// other moveable
		else if (other.movement) {
			n *= -1.0f;
			other.velocity -= n * 2.0f * dotProduct(other.velocity, n);
		}
	}

}
