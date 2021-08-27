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
		void resetCameraState();

		void setFov(int fov);

		void setpos(float x, float y, float z);
		void setrot(float x, float y, float z);
		void move(float x, float y, float z);
		void rotate(float x, float y, float z);

		Vec3f getCameraCoord(Vec3f in) const;
		Vec2 getScreenCoord(const Vec3f& in) const;

		void getControls();
	};

}

#endif
