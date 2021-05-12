#ifndef UTIL_H_
#define UTIL_H_


#include "render.h"


inline int range(int num, int min, int max) {
	if (num < min) return min;
	if (num > max) return max;
	return num;
}


inline void swapInt(int* num1, int* num2) {
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}


inline int rgbToDec(int r, int g, int b) {
	return (r << 16) + (g << 8) + b;
}


inline void sortTriToRaster(struct Tri2D* tri) {
	// sort points by y ascending
	if (tri->pts[0].y > tri->pts[1].y) {
		swapInt(&tri->pts[0].y, &tri->pts[1].y);
		swapInt(&tri->pts[0].x, &tri->pts[1].x);
	}
	if (tri->pts[0].y > tri->pts[2].y) {
		swapInt(&tri->pts[0].y, &tri->pts[2].y);
		swapInt(&tri->pts[0].x, &tri->pts[2].x);
	}
	if (tri->pts[1].y > tri->pts[2].y) {
		swapInt(&tri->pts[1].y, &tri->pts[2].y);
		swapInt(&tri->pts[1].x, &tri->pts[2].x);
	}
}


inline struct Vec3f addVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.x + bVec.x;
	out.y = aVec.y + bVec.y;
	out.z = aVec.z + bVec.z;
	return out;
}

inline struct Vec3f subtractVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.x - bVec.x;
	out.y = aVec.y - bVec.y;
	out.z = aVec.z - bVec.z;
	return out;
}


inline struct Vec3f scalarMultVec3(struct Vec3f in, float mult) {
	struct Vec3f out = { 0.0f };
	out.x = in.x * mult;
	out.y = in.y * mult;
	out.z = in.z * mult;
	return out;
}


inline struct Vec3f scalarDivVec3(struct Vec3f in, float divisor) {
	struct Vec3f out = { 0.0f };
	out.x = in.x / divisor;
	out.y = in.y / divisor;
	out.z = in.z / divisor;
	return out;
}


inline float dotProductVec3(struct Vec3f aVec, struct Vec3f bVec) {
	return aVec.x * bVec.x + aVec.y * bVec.y + aVec.z * bVec.z;
}


inline struct Vec3f crossProductVec3(struct Vec3f aVec, struct Vec3f bVec) {
	struct Vec3f out = { 0.0f };
	out.x = aVec.y * bVec.z - aVec.z * bVec.y;
	out.y = aVec.z * bVec.x - aVec.x * bVec.z;
	out.z = aVec.x * bVec.y - aVec.y * bVec.x;
	return out;
}


inline struct Vec3f normalizeVector(struct Vec3f in) {
	struct Vec3f out = { 0.0f };
	float mag = 0.0f;
	mag = sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
	out = scalarDivVec3(in, mag);
	return out;
}


inline float similarityVec3(Vec3f aVec, Vec3f bVec) {
	float similarity = 0.0f;
	aVec = normalizeVector(aVec);
	bVec = normalizeVector(bVec);
	similarity = dotProductVec3(aVec, bVec);
	similarity = (similarity + 1.0f) / 2.0f;
	return similarity;
}


inline struct Vec3f multiplyMatrixVec3(struct Matrix3* mat, struct Vec3f* in) {
	struct Vec3f out = { 0.0f };
	out.x = in->x * mat->num[0][0] + in->y * mat->num[0][1] + in->z * mat->num[0][2];
	out.y = in->x * mat->num[1][0] + in->y * mat->num[1][1] + in->z * mat->num[1][2];
	out.z = in->x * mat->num[2][0] + in->y * mat->num[2][1] + in->z * mat->num[2][2];
	return out;
}


#endif
