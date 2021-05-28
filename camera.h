#ifndef CAMERA_H_
#define CAMERA_H_


#include "vec.h"
#include <math.h>


class Camera {
public:
	struct Vec3f pos;
	struct Vec3f rot;

	float sinx;
	float siny;
	float sinz;

	float cosx;
	float cosy;
	float cosz;

	void calcTrigValues() {
		sinx = sinf(rot.x);
		siny = sinf(rot.y);
		sinz = sinf(rot.z);

		cosx = cosf(rot.x);
		cosy = cosf(rot.y);
		cosz = cosf(rot.z);
	}

	Camera(Vec3f aPos, Vec3f aRot) {
		pos = aPos;
		rot.x = aRot.x * 3.14159f / 180.0f;
		rot.y = aRot.y * 3.14159f / 180.0f;
		rot.z = aRot.z * 3.14159f / 180.0f;
		calcTrigValues();
	}

	void resetCameraState() {
		pos = { 0.0f };
		rot = { 0.0f };
	}

	void setPos(float xNew, float yNew, float zNew) {
		pos.x = xNew;
		pos.y = yNew;
		pos.z = zNew;
	}

	void setRot(float xNew, float yNew, float zNew) {
		rot.x = xNew;
		rot.y = yNew;
		rot.z = zNew;
	}
};


#endif
