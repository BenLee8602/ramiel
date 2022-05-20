#ifndef BL_PHYSICS_H
#define BL_PHYSICS_H

#include "vec.h"

namespace bl {

	struct Physics {
		Vec3f pos       = vec3f_0;
		Vec3f rot       = vec3f_0;
		bool  collision = false;
		float hbxrad    = 0.5f;
		float mass      = 1.0f;
		bool  movement  = false;
		Vec3f posVel    = vec3f_0;
		Vec3f posAcc    = vec3f_0;
		Vec3f rotVel    = vec3f_0;
		Vec3f rotAcc    = vec3f_0;
		
		Physics(
			Vec3f pos       = vec3f_0,
			Vec3f rot       = vec3f_0,
			bool  collision = false,
			float hbxrad    = 0.5f,
			float mass      = 1.0f,
			bool  movement  = false,
			Vec3f posVel    = vec3f_0,
			Vec3f posAcc    = vec3f_0,
			Vec3f rotVel    = vec3f_0,
			Vec3f rotAcc    = vec3f_0
		) :
			pos(pos),
			rot(rot),
			collision(collision),
			hbxrad(hbxrad),
			mass(mass),
			movement(movement),
			posVel(posVel),
			posAcc(posAcc),
			rotVel(rotVel),
			rotAcc(rotAcc)
		{}

		void simulateMovement();
		void simulateCollision(Physics& other);
	};

}

#endif
