#ifndef BL_PHYSICS_H
#define BL_PHYSICS_H

#include "vec.h"

namespace bl {

	struct Physics {
		Vec3f pos          = vec3f_0;
		bool  collision    = false;
		float hbxrad       = 0.5f;
		float mass         = 1.0f;
		bool  movement     = false;
		Vec3f velocity     = vec3f_0;
		Vec3f acceleration = vec3f_0;
		
		Physics(
			Vec3f pos          = vec3f_0,
			bool  collision    = false,
			float hbxrad       = 0.5f,
			float mass         = 1.0f,
			bool  movement     = false,
			Vec3f velocity     = vec3f_0,
			Vec3f acceleration = vec3f_0
		) :
			pos(pos),
			collision(collision),
			hbxrad(hbxrad),
			mass(mass),
			movement(movement),
			velocity(velocity),
			acceleration(acceleration)
		{}

		void simulateMovement();
		void simulateCollision(Physics& other);
	};

}

#endif
