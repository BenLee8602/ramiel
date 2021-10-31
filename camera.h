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
		Camera();
		Camera(const Vec3f& pos, const Vec3f& rot, int fov = 0);
		void calcTrigValues();
		void reset();
		
		void setFov(int fov);
		
		const Vec3f& getpos() const;
		const Vec3f& getrot() const;

		void move(float _x, float _y, float _z);
		void rotate(float _x, float _y, float _z);

		Vec3f getCameraCoord(Vec3f in) const;
		Vec2 getScreenCoord(const Vec3f& in) const;

		void getControls();
	};

}

#endif
