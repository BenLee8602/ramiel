#ifndef RENDER_H_
#define RENDER_H_


#include <math.h>
#include <vector>


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
	struct Vec3f pts[3];
};


struct Matrix3 {
	float num[3][3] = { 0.0f };
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


class Line {
public:
	Vec2 start = { 0 };
	Vec2 end = { 0 };
	float slope = 0.0f;
	float invSlope = 0.0f;

	Line(Vec2 aStart, Vec2 aEnd) {
		start = aStart;
		end = aEnd;
		slope = (float)(end.y - start.y) / (float)(end.x - start.x);
		invSlope = 1.0f / slope;
	}

	int getXatY(int y) {
		if (end.y == start.y) return 0;
		return start.x + (end.x - start.x) * (y - start.y) / (end.y - start.y);
	}

	int getYatX(int x) {
		if (end.x == start.x) return 0;
		return start.y + (end.y - start.y) * (x - start.x) / (end.x - start.x);
	}

	int clipX(int min, int max) {
		int lineState = 0;

		// clip start point
		if (start.x < min) {
			lineState += 1;
			start = { min, getYatX(min) };
		}
		else if (start.x > max) {
			lineState += 2;
			start = { max, getYatX(max) };
		}

		// clip end point
		if (end.x < min) {
			lineState += 4;
			end = { min, getYatX(min) };
		}
		else if (end.x > max) {
			lineState += 8;
			end = { max, getYatX(max) };
		}

		return lineState;
	}

	int clipY(int min, int max) {
		int lineState = 0;

		// clip start point
		if (start.y < min) {
			lineState += 1;
			start = { getXatY(min), min };
		}
		else if (start.y > max) {
			lineState += 2;
			start = { getXatY(max), max };
		}

		// clip end point
		if (end.y < min) {
			lineState += 4;
			end = { getXatY(min), min };
		}
		else if (end.y > max) {
			lineState += 8;
			end = { getXatY(max), max };
		}

		return lineState;
	}
};


struct Vec2 getScreenCoords(struct Vec3f* pt3D);
void clipTriangle(std::vector<Tri2D> &clippedTris, Tri2D triInput);

void renderMain();
void renderStart();


#endif
