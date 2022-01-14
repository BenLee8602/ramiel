#ifndef BL_CAMERA_H
#define BL_CAMERA_H

#include "vec.h"

namespace bl {

	class Camera {
		Vec3f pos;
		Vec3f rot;

		Vec3f sin;
		Vec3f cos;

		int focalLen;

	public:

		float znear;
		float zfar;

		Camera(
			int fov = 0,
			float znear = 0.2f,
			float zfar = 1000.0f
		);
		void calcTrigValues();
		void reset();
		
		void setFov(int fov);
		
		const Vec3f& getpos() const;
		const Vec3f& getrot() const;

		void move(float x, float y, float z);
		void rotate(float x, float y, float z);

		Vec3f getCameraCoord(Vec3f in) const;
		Vec2 getScreenCoord(const Vec3f& in) const;

		void setControls(bool controls[12]);
	};

}

#endif
