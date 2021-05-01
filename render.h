#ifndef RENDER_H_
#define RENDER_H_


#include <math.h>


struct Vec2 {
	int x;
	int y;
};

struct Vec3 {
	int x;
	int y;
	int z;
};


struct Vec2f {
	float x;
	float y;
};

struct Vec3f {
	float x;
	float y;
	float z;
};


struct Tri2D {
	struct Vec2 pts[3];
};

struct Tri3D {
	struct Vec3 pts[3];
};


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


struct Matrix3 {
	float num[3][3] = { 0.0f };
};


struct Vec2 getScreenCoords(struct Vec3* pt3D);
void renderMain();
void renderStart();


#endif
