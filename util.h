#ifndef UTIL_H_
#define UTIL_H_


#include "render.h"


int range(int num, int min, int max) {
	if (num < min) return min;
	if (num > max) return max;
	return num;
}


struct Vec3f vectorMatrixMult3(struct Matrix3* mat, struct Vec3f* in) {
	struct Vec3f out = { 0.0f };
	out.x = in->x * mat->num[0][0] + in->y * mat->num[0][1] + in->z * mat->num[0][2];
	out.y = in->x * mat->num[1][0] + in->y * mat->num[1][1] + in->z * mat->num[1][2];
	out.z = in->x * mat->num[2][0] + in->y * mat->num[2][1] + in->z * mat->num[2][2];
	return out;
}


#endif
