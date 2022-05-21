#pragma once

#include "vec.h"

namespace ramiel {

	class Camera {
	#ifdef RAMIEL_TEST
	public:
	#endif
		Vec3f pos; 
		Vec3f rot;

		Vec3f sin;
		Vec3f cos;

		float focalLen;

	public:

		float znear;
		float zfar;

		Camera(
			unsigned fov = 0,
			float znear = 0.2f,
			float zfar = 1000.0f
		);
		void calcTrigValues();
		void reset();
		
		void setFov(unsigned fov);
		
		const Vec3f& getpos() const;
		const Vec3f& getrot() const;

		void setpos(const Vec3f& pos);
		void setrot(const Vec3f& rot);

		void move(float x, float y, float z);
		void rotate(float x, float y, float z);

		Vec3f getCameraCoord(Vec3f in) const;
		Vec2 getScreenCoord(const Vec3f& in) const;

		void setControls(bool controls[12]);
	};

}
