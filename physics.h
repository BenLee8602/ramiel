#ifndef BL_PHYSICS_H
#define BL_PHYSICS_H

#include "vec.h"

namespace bl {

	struct Physics {
		bool  collision    = false;
		bool  movement     = false;
		float hbxrad       = 0.5f;
		float mass         = 1.0f;
		Vec3f pos          = vec3f_0;
		Vec3f velocity     = vec3f_0;
		Vec3f acceleration = vec3f_0;
		
		// position only constructor
		Physics(Vec3f pos) : pos(pos) {}

		// collision only constructor
		Physics(
			Vec3f pos,
			float hbxrad
		) :
			collision(true),
			hbxrad(hbxrad),
			pos(pos)
		{}

		// movement only constructor
		Physics(
			float mass,
			Vec3f pos,
			Vec3f velocity,
			Vec3f acceleration
		) :
			movement(true),
			mass(mass),
			pos(pos),
			velocity(velocity),
			acceleration(acceleration)
		{}

		// collision and movement, default
		Physics(
			bool  collision    = false,
			bool  movement     = false,
			float hbxrad       = 0.5f,
			float mass         = 1.0f,
			Vec3f pos          = vec3f_0,
			Vec3f velocity     = vec3f_0,
			Vec3f acceleration = vec3f_0
		) :
			collision(collision),
			movement(movement),
			hbxrad(hbxrad),
			mass(mass),
			pos(pos),
			velocity(velocity),
			acceleration(acceleration)
		{}

		void simulateMovement();
		void simulateCollision(Physics& other);
	};

}

#endif
