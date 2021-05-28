#ifndef LINE_H_
#define LINE_H_


#include "vec.h"


class Line2D {
public:
	Vec2 start = { 0 };
	Vec2 end = { 0 };
	float slope = 0.0f;
	float invSlope = 0.0f;

	Line2D(Vec2 aStart, Vec2 aEnd) {
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


// for z-axis clipping
class Line3D {
public:
	Vec3f start = { 0.0f };
	Vec3f end = { 0.0f };
	float slope = 0.0f;		// when line projected onto xy axis
	float invSlope = 0.0f;

	Line3D(Vec3f aStart, Vec3f aEnd) {
		start = aStart;
		end = aEnd;
		slope = (float)(end.y - start.y) / (float)(end.x - start.x);
		invSlope = 1.0f / slope;
	}

	Vec2 getXYatZ(float z) {

	}
};


#endif
